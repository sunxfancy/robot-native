/*
 * @CreateTime: Apr 26, 2017 8:58 PM 
 * @Author: sxf 
 * @Contact: sunxfancy@gmail.com 
 * @Last Modified By: sxf
 * @Last Modified Time: Apr 26, 2017 9:18 PM
 * @Description: Modify Here, Please  
 */

#include <dlib/image_processing/frontal_face_detector.h>
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
 * detect(face: ImageData): [{x,y,width,height}]
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

} // namespace robot