/*
 * @CreateTime: Apr 26, 2017 8:58 PM 
 * @Author: sxf 
 * @Contact: sunxfancy@gmail.com 
 * @Last Modified By: sxf
 * @Last Modified Time: Apr 26, 2017 9:18 PM
 * @Description: Modify Here, Please  
 */

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/shape_predictor.h>
#include <node.h>
#include <vector>
#include <iostream>

using namespace dlib;
using namespace v8;

namespace robot {

void loadImageData(array2d<rgb_pixel>& img, const Local<Object>& input, Isolate* isolate) {
    Local<Integer> v_width = input->Get(String::NewFromUtf8(isolate, "width"))->ToInteger();
    Local<Integer> v_height = input->Get(String::NewFromUtf8(isolate, "height"))->ToInteger();
    Local<Uint8ClampedArray> v_data = Local<Uint8ClampedArray>::Cast(input->Get(String::NewFromUtf8(isolate, "data")));
    int width  = v_width->NumberValue();
    int height = v_height->NumberValue();
    const unsigned char *data = (unsigned char *)v_data->Buffer()->GetContents().Data();
    img.set_size(width, height);
    for (int i = 0; i< height; ++i) {
        for (int j = 0; j< width; ++j) {
            int p = (i*width+j)*4;
            img[i][j].red = data[p];
            img[i][j].green = data[p+1];
            img[i][j].blue = data[p+2];
            // rgb_pixel(data[p], data[p+1], data[p+2]);
        }
    }
    // pyramid_up(img);

}

/**
 * detectFace(face: ImageData): [{x,y,width,height}]
 * ImageData: {width, height, data: uchar[]}
 */
extern void
DetectFace(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Object> input = args[0]->ToObject();
    static frontal_face_detector detector = get_frontal_face_detector();
    array2d<rgb_pixel> img;
    // load_image(img, argv[i]);
    loadImageData(img, input, isolate);
    std::vector<rectangle> dets = detector(img);

    Local<Array> array = Array::New(isolate, dets.size());
    for (int i=0; i< dets.size(); ++i) {
        rectangle& rect = dets[i];
        Local<Object> obj = Object::New(isolate);
        obj->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, rect.left()));
        obj->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, rect.top()));
        obj->Set(String::NewFromUtf8(isolate, "width"), Number::New(isolate, rect.width()));
        obj->Set(String::NewFromUtf8(isolate, "height"), Number::New(isolate, rect.height()));
        array->Set(i, obj);
    }
    args.GetReturnValue().Set(array);
}

/**
 * detectDetail(face: ImageData): [{x,y,width,height,points:array[68]}]
 * ImageData: {width, height, data: uchar[]}
 */
extern void
DetectDetail(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Object> input = args[0]->ToObject();
    static frontal_face_detector detector = get_frontal_face_detector();
    array2d<rgb_pixel> img;
    // load_image(img, argv[i]);
    loadImageData(img, input, isolate);
    std::vector<rectangle> dets = detector(img);
    if (dets.empty()) {
        args.GetReturnValue().Set(Array::New(isolate, 0));
        return;
    }
    // load 100MB model 
    static dlib::shape_predictor pose_model;  
    static bool bFirst = true;
    if (bFirst) {
        std::cout << "start loading" << std::endl;
        bFirst = false;
        deserialize("./assets/shape_predictor_68_face_landmarks.dat") >> pose_model;
        std::cout << "data sets loaded." << std::endl;
    }

    Local<Array> array = Array::New(isolate, dets.size());
    for (int i=0; i< dets.size(); ++i) {
        rectangle& rect = dets[i];
        Local<Object> obj = Object::New(isolate);
        obj->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, rect.left()));
        obj->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, rect.top()));
        obj->Set(String::NewFromUtf8(isolate, "width"), Number::New(isolate, rect.width()));
        obj->Set(String::NewFromUtf8(isolate, "height"), Number::New(isolate, rect.height()));

        full_object_detection points = pose_model(img, rect);
        Local<Array> parray = Array::New(isolate, points.num_parts());
        for (unsigned int j=0; j < points.num_parts(); j++) {
            Local<Object> p = Object::New(isolate);
            p->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, points.part(j).x()));
            p->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, points.part(j).y()));
            parray->Set(j, p);
        }
        obj->Set(String::NewFromUtf8(isolate, "points"), parray);
        
        array->Set(i, obj);
    }
    args.GetReturnValue().Set(array);
}

} // namespace robot