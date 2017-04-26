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
    unsigned char *data = (unsigned char *)v_data->Buffer()->GetContents().Data();
    img.set_size(width, height);
    
    std::cout << width << ' ' << height << ' ' << (int)data[0]<< std::endl;
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
    Local<Object> obj = Object::New(isolate);
    args.GetReturnValue().Set(array);
}

} // namespace robot