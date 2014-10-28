#DrawScore.

An app that lets users read hand-drawn and musical scores with their mobile device.

There is 2 branches, one for OSX, the other for Android, as developing for OSX in XCode is a lot faster the developing in Eclipse with the Android NDK.

The app uses the camera torch on android, so you'll need to patch your ofxAndroid addon:

in `openFrameworks/addons/ofxAndroid/src/ofxAndroidVideoGrabber.h`
add the following public method:

```c++
bool setTorchActivated(bool torchActivated);
```

in `openFrameworks/addons/ofxAndroid/src/ofxAndroidVideoGrabber.cpp`
add its implementation:

```c++
bool ofxAndroidVideoGrabber::setTorchActivated(bool torchActivated){
	JNIEnv *env = ofGetJNIEnv();
	if(!env) return false;
    
	jclass javaClass = getJavaClass();
    
	jobject camera = getCamera(env, javaClass, cameraId);
	jmethodID javasetTorchActivated = env->GetMethodID(javaClass,"setTorchActivated","(Z)Z");
	if(camera && javasetTorchActivated){
		return env->CallBooleanMethod(camera,javasetTorchActivated,torchActivated);
	}else{
		ofLogError("ofxAndroidVideoGrabber") << "setTorchActivated(): couldn't get OFAndroidVideoGrabber setTorchActivated method";
		return false;
	}
}
```

in `openFrameworks/addons/ofxAndroid/ofAndroidLib/src/cc/openframeworks/OFAndroidVideoGrabber.java`
add the following method:

```java
public boolean setTorchActivated(boolean torchActivated){
	Camera.Parameters config = camera.getParameters();
	config.setFlashMode(torchActivated ? Camera.Parameters.FLASH_MODE_TORCH : Camera.Parameters.FLASH_MODE_OFF);
	try{
		camera.setParameters(config);
	}catch(Exception e){
		Log.e("OF","could not set Camera flash mode", e);
		return false;
	}
	return true;
}
```


