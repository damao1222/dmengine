LOCAL_PATH :=  $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := dmengine_static

LOCAL_SRC_FILES := \
 	dmengine/core/dmapplication.cpp \
  	dmengine/core/dmautoreleasemgr.cpp \
  	dmengine/core/dmbroadcast.cpp \
  	dmengine/core/dmbroadcastfilter.cpp \
  	dmengine/core/dmbroadcastreceiver.cpp \
	dmengine/core/dmcoreapplication.cpp \
	dmengine/core/dmeventfilter.cpp \
 	dmengine/core/dminteractiveobject.cpp \
 	dmengine/core/dmliveobject.cpp \
	dmengine/core/dmmalloc.cpp \
  	dmengine/core/dmobject.cpp \
  	dmengine/core/dmobserver.cpp \
  	dmengine/core/dmtimer.cpp \
  	dmengine/core/events/dmpostevent.cpp \
  	dmengine/core/input/dmtouchdispatcher.cpp \
  	dmengine/core/private/dmliveobject_p.cpp \
  	dmengine/core/thread/dmatomic.cpp \
	dmengine/core/thread/dmcondition.cpp \
	dmengine/core/thread/dmexecutor.cpp \
	dmengine/core/thread/dmreadwritelock.cpp \
	dmengine/core/thread/dmthread.cpp \
	dmengine/core/thread/unix/dmthread_pthread.cpp \
	dmengine/filesystem/dmdir.cpp \
	dmengine/filesystem/dmfile.cpp \
	dmengine/filesystem/dmfileinfo.cpp \
	dmengine/filesystem/dmifile.cpp \
	dmengine/filesystem/fileformat/json/dmjsonbuilder.cpp \
	dmengine/filesystem/fileformat/json/dmjsonreader.cpp \
	dmengine/filesystem/fileformat/json/dmjsonwriter.cpp \
	dmengine/filesystem/platform/dmfilehelper.cpp \
	dmengine/filesystem/platform/unix/dmfileunix.cpp \
	dmengine/graphics/dmcircle.cpp \
	dmengine/graphics/dmdirection.cpp \
	dmengine/graphics/dmgraphicsitem.cpp \
	dmengine/graphics/dmgraphicsmath.cpp \
	dmengine/graphics/dmgraphicsobjectproxy.cpp \
	dmengine/graphics/dmline.cpp \
	dmengine/graphics/dmpoint.cpp \
	dmengine/graphics/dmrect.cpp \
	dmengine/graphics/dmsize.cpp \
	dmengine/network/dmsocket.cpp \
	dmengine/network/http/httpclient.cpp \
	dmengine/public/dmcow.cpp \
	dmengine/public/dmmath.cpp \
	dmengine/public/dmrefcount.cpp \
	dmengine/utils/dmbuiltin.cpp \
	dmengine/utils/dmchar.cpp \
	dmengine/utils/dmcharsetconverter.cpp \
	dmengine/utils/dmcollision.cpp \
	dmengine/utils/dmdatabuffer.cpp \
	dmengine/utils/dmendtime.cpp \
	dmengine/utils/dmeventhandler.cpp \
	dmengine/utils/dmgraphicsutils.cpp \
	dmengine/utils/dmhash.cpp \
	dmengine/utils/dmlogger.cpp \
	dmengine/utils/dmmetatype.cpp \
	dmengine/utils/dmprobabilityutil.cpp \
	dmengine/utils/dmrandom.cpp \
	dmengine/utils/dmrefarray.cpp \
	dmengine/utils/dmstringtypeutils.cpp \
	dmengine/utils/dmtime.cpp \
	dmengine/utils/dmutilstring.cpp \
	dmengine/utils/dmvariant.cpp \
	dmengine/utils/dmvector.cpp \
	dmengine/utils/dmzonehelper.cpp \
	dmengine/utils/md5/dmmd5.cpp \
	dmengine/utils/plugin/dmclassloader.cpp \
	dmengine/utils/plugin/dmlibrary.cpp \
	dmengine/utils/plugin/dmlibraryloader.cpp \
	dmengine/utils/plugin/private/dmdllloader_unix.cpp \
	dmengine/utils/private/dmrefarray_p.cpp \
	dmengine/utils/private/dmutilstring_p.cpp \
	dmengine/utils/thread/dmatomicarray.cpp \
	dmengine/utils/thread/dmloopthread.cpp \
	dmengine/utils/thread/arch/common/dmpoolthread.cpp \
	dmengine/utils/thread/arch/common/dmthreadpool_common.cpp \
	dmengine/windowsystem/dmwidget.cpp \
	dmengine/windowsystem/dmwindowsystem.cpp
  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/dmengine/core \
	$(LOCAL_PATH)/dmengine/core/arch \
	$(LOCAL_PATH)/dmengine/core/events \
	$(LOCAL_PATH)/dmengine/core/inptu \
	$(LOCAL_PATH)/dmengine/core/private \
	$(LOCAL_PATH)/dmengine/core/thread \
	$(LOCAL_PATH)/dmengine/filesystem \
	$(LOCAL_PATH)/dmengine/utils \
	$(LOCAL_PATH)/dmengine/graphics \
	$(LOCAL_PATH)/dmengine/network \
	$(LOCAL_PATH)/dmengine/public \
	$(LOCAL_PATH)/dmengine/windowsystem

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/dmengine/public \
	$(LOCAL_PATH)/dmengine/network \
	$(LOCAL_PATH)/dmengine/utils \
	$(LOCAL_PATH)/dmengine/core \
	$(LOCAL_PATH)/dmengine/graphics \
	$(LOCAL_PATH)/dmengine/filesystem

LOCAL_LDLIBS += -llog -lpthread 
LOCAL_EXPORT_LDLIBS := -llog
LOCAL_CFLAGS := -fexceptions
#LOCAL_CFLAGS += -DDM_NO_EXCEPTIONS
TARGET_CFLAGS := -mcpu=cortex-a9


include $(BUILD_STATIC_LIBRARY)