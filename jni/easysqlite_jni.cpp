#include "JNIHelp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include "CppSQLite3.h"
//#include <android_runtime/AndroidRuntime.h>

//#define LOG_TAG "mysqlite"


//指定要注册的类
#define JNIREG_CLASS "easysqlite/SQLiteCore"


static CppSQLite3DB* myDB = 0;
typedef struct _queryResultItem
{
	void* addr;
}queryResultItem;

static queryResultItem qResult = {NULL};


CppSQLite3DB* newSQLite();
void deleteSQLite();
CppSQLite3DB* open(char* dbPath);

CppSQLite3DB* newSQLite()
{
	if(myDB)
	{
		deleteSQLite();
	}
	LOGD("newSQLite myDB");
	return new CppSQLite3DB();
}

void deleteSQLite()
{
	if(myDB)
	{
		delete myDB;
		myDB = 0;
		LOGD("deleteSQLite myDB");
	}
}

CppSQLite3DB* open(char* dbPath)
{
	if(!dbPath)
	{
		return NULL;
	}
	newSQLite();
	myDB->open(dbPath);
}

int setQueryResultItem(CppSQLite3Query query)
{
	CppSQLite3Query* obj = new CppSQLite3Query(query);
	if(qResult.addr)
	{
		CppSQLite3Query* t = (CppSQLite3Query*)qResult.addr;
		delete t;
		qResult.addr = NULL;
	}
	qResult.addr = obj;
	return (int)qResult.addr;
}

CppSQLite3Query* getQueryResultItem(int pointer)
{
	void* ptr = (void*)pointer;
	if(ptr == qResult.addr)
	{
		CppSQLite3Query* t = (CppSQLite3Query*)qResult.addr;
		return t;
	}
	else
	{
		return NULL;
	}
}

void clearQueryResultItem(void)
{
	if(qResult.addr)
	{
		CppSQLite3Query* t = (CppSQLite3Query*)qResult.addr;
		delete t;
		qResult.addr = NULL;
	}
}

JNIEXPORT jstring JNICALL easysqlite_version
  (JNIEnv *env, jclass jcs)
{
	jstring ret = env->NewStringUTF(SQLITE_VERSION);
	return ret;
}

JNIEXPORT jint JNICALL easysqlite_open
  (JNIEnv *env, jobject obj, jstring filePath){
  	const char *dbPath = env->GetStringUTFChars(filePath, 0);
  	
  	jint ret = SQLITE_OK;
  	if(dbPath)
  	{
  		LOGD("easysqlite_open dbPath:%s", dbPath);
  		myDB = newSQLite();
  		try
  		{
  			myDB->open(dbPath);
  		}
  		catch (CppSQLite3Exception& e)
	    {
	    	deleteSQLite();
	    	ret = SQLITE_ERROR;
	    	LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
	    }
  	}
	else
	{
		LOGD("easysqlite_open dbPath null");
		ret = SQLITE_ERROR;
	}

  	env->ReleaseStringUTFChars(filePath, dbPath);
  	return ret;
}

JNIEXPORT jint JNICALL easysqlite_openWithPassword
  (JNIEnv *env, jobject obj, jstring filePath, jstring password){
  	const char *dbPath = env->GetStringUTFChars(filePath, 0);
	const char *dbPass = env->GetStringUTFChars(password, 0);
  	
  	jint ret = SQLITE_OK;
  	if(dbPath)
  	{
  		LOGD("easysqlite_open dbPath:%s", dbPath);
  		myDB = newSQLite();
  		try
  		{
  			myDB->open(dbPath);
			myDB->setkey(dbPass);
  		}
  		catch (CppSQLite3Exception& e)
	    {
	    	deleteSQLite();
	    	ret = SQLITE_ERROR;
	    	LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
	    }
  	}
	else
	{
		LOGD("easysqlite_open dbPath null");
		ret = SQLITE_ERROR;
	}

  	env->ReleaseStringUTFChars(filePath, dbPath);
	env->ReleaseStringUTFChars(password, dbPass);
  	return ret;
}

JNIEXPORT void JNICALL easysqlite_close
  (JNIEnv *env, jobject obj){
  	if(myDB)
  	{
  		if(myDB->checkDBIsOpen())
  		{
			myDB->close();
			LOGD("easysqlite_close myDB close");
		}
  	}
}

JNIEXPORT void JNICALL easysqlite_clear
  (JNIEnv *env, jobject obj)
{
	deleteSQLite();
	LOGD("easysqlite_clear myDB clear");
}

JNIEXPORT jint JNICALL easysqlite_execDML
  (JNIEnv *env, jobject obj, jstring sql){
  	const char *charsql = env->GetStringUTFChars(sql, 0);
	jint ret = SQLITE_OK;
	if(charsql)
	{
		LOGD("easysqlite_execDML sql: %s", charsql);
		try
		{
			ret = myDB->execDML(charsql);
		}
		catch (CppSQLite3Exception& e)
		{
			ret = SQLITE_ERROR;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_execDML sql null");
		ret = SQLITE_ERROR;
	}
	
	env->ReleaseStringUTFChars(sql, charsql);
  	return ret;
}

JNIEXPORT jboolean JNICALL easysqlite_setKey
  (JNIEnv *env, jobject obj, jstring key){
  	const char *charkey = env->GetStringUTFChars(key, 0);
	jboolean ret = true;
	if(charkey)
	{
		LOGD("easysqlite_setkey key: %s %d", charkey, strlen(charkey));
		try
		{
			ret = myDB->setkey("");
			ret = myDB->rekey(charkey);
			ret = myDB->setkey(charkey);
		}
		catch (CppSQLite3Exception& e)
		{
			ret = false;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_setkey key null");
		ret = false;
	}
	
	env->ReleaseStringUTFChars(key, charkey);
  	return ret;
}

JNIEXPORT jboolean JNICALL easysqlite_clearKey
  (JNIEnv *env, jobject obj, jstring key){
  	const char *charkey = env->GetStringUTFChars(key, 0);
	jboolean ret = true;
	if(charkey)
	{
		LOGD("easysqlite_clearKey key: %s %d", charkey, strlen(charkey));
		try
		{
			ret = myDB->setkey(charkey);
			ret = myDB->rekey("");
			ret = myDB->setkey("");
		}
		catch (CppSQLite3Exception& e)
		{
			ret = false;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_clearKey key null");
		ret = false;
	}
	
	env->ReleaseStringUTFChars(key, charkey);
  	return ret;
}

JNIEXPORT jboolean JNICALL easysqlite_changeKey
  (JNIEnv *env, jobject obj, jstring oldKey, jstring newKey){
  	const char *charOldKey = env->GetStringUTFChars(oldKey, 0);
	const char *charNewKey = env->GetStringUTFChars(newKey, 0);
	jboolean ret = true;
	if(charOldKey && charNewKey)
	{
		LOGD("easysqlite_changeKey charOldKey: %s %d charNewKey: %s %d", charOldKey, strlen(charOldKey), charNewKey, strlen(charNewKey));
		try
		{
			ret = myDB->setkey(charOldKey);
			ret = myDB->rekey(charNewKey);
			ret = myDB->setkey(charNewKey);
		}
		catch (CppSQLite3Exception& e)
		{
			ret = false;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_changeKey charOldKey or charNewKey null");
		ret = false;
	}
	
	env->ReleaseStringUTFChars(oldKey, charOldKey);
	env->ReleaseStringUTFChars(newKey, charNewKey);
  	return ret;
}

JNIEXPORT jint JNICALL easysqlite_execQuery
  (JNIEnv *env, jobject obj, jstring sql){
  	const char *charsql = env->GetStringUTFChars(sql, 0);
	jint ret = SQLITE_OK;
	if(charsql)
	{
		LOGD("easysqlite_execQuery sql: %s", charsql);
		try
		{
			ret = setQueryResultItem(myDB->execQuery(charsql));
			if(ret != SQLITE_ERROR)
			{
				LOGD("easysqlite_execQuery success");
			}
		}
		catch (CppSQLite3Exception& e)
		{
			ret = SQLITE_ERROR;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_execQuery sql null");
		ret = SQLITE_ERROR;
	}
	
	env->ReleaseStringUTFChars(sql, charsql);
  	return ret;
}

JNIEXPORT jboolean JNICALL easysqlite_eof
  (JNIEnv *env, jobject obj, jint pointer)

{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	jboolean ret = false;
	if(t)
	{
		try
		{
			ret = t->eof();
		}
		catch (CppSQLite3Exception& e)
		{
			ret = false;
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_eof no pointer");
	}

    return ret;
}

JNIEXPORT void JNICALL easysqlite_nextRow
  (JNIEnv *env, jobject obj, jint pointer)

{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	if(t)
	{
		try
		{
			t->nextRow();
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_nextRow no pointer");
	}
}

JNIEXPORT jstring JNICALL easysqlite_fieldValueWithName
  (JNIEnv *env, jobject obj, jint pointer, jstring name)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	const char* ret = NULL;
	const char *charname = env->GetStringUTFChars(name, 0);
	if(t)
	{
		try
		{
			ret = t->fieldValue(charname);
			LOGD("easysqlite_fieldValueWithName sql: %s", ret);
			
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_fieldValueWithName no pointer");
	}

	env->ReleaseStringUTFChars(name, charname);

	jstring result = env->NewStringUTF(ret);
	return result;
}

JNIEXPORT jstring JNICALL easysqlite_fieldValueWithIndex
  (JNIEnv *env, jobject obj, jint pointer, jint index)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	const char* ret = NULL;
	if(t)
	{
		try
		{
			ret = t->fieldValue(index);
			LOGD("easysqlite_fieldValueWithIndex sql: %s", ret);
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_fieldValueWithIndex no pointer");
	}

	jstring result = env->NewStringUTF(ret);
	return result;
}

JNIEXPORT jstring JNICALL easysqlite_getStringFieldValueWithName
  (JNIEnv *env, jobject obj, jint pointer, jstring name)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	const char* ret = NULL;
	const char *charname = env->GetStringUTFChars(name, 0);
	if(t)
	{
		try
		{
			ret = t->getStringField(charname);
			LOGD("easysqlite_getStringfieldValueWithName sql: %s", ret);
			
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_getStringfieldValueWithName no pointer");
	}

	env->ReleaseStringUTFChars(name, charname);

	jstring result = env->NewStringUTF(ret);
	return result;
}


JNIEXPORT jstring JNICALL easysqlite_getStringFieldValueWithIndex
  (JNIEnv *env, jobject obj, jint pointer, jint index)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	const char* ret = NULL;
	if(t)
	{
		try
		{
			ret = t->getStringField(index);
			LOGD("easysqlite_execQuery sql: %s", ret);
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_fieldValueWithIndex no pointer");
	}

	jstring result = env->NewStringUTF(ret);
	return result;
}

JNIEXPORT jint JNICALL easysqlite_getIntFieldValueWithName
  (JNIEnv *env, jobject obj, jint pointer, jstring name)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	jint ret = 0;
	const char *charname = env->GetStringUTFChars(name, 0);
	if(t)
	{
		try
		{
			ret = t->getIntField(charname);
			LOGD("easysqlite_getIntfieldValueWithName sql: %d", ret);
			
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_getIntfieldValueWithName no pointer");
	}

	env->ReleaseStringUTFChars(name, charname);

	return ret;
}


JNIEXPORT jint JNICALL easysqlite_getIntFieldValueWithIndex
  (JNIEnv *env, jobject obj, jint pointer, jint index)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	jint ret = 0;
	if(t)
	{
		try
		{
			ret = t->getIntField(index);
			LOGD("easysqlite_getIntfieldValueWithName sql: %d", ret);
			
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_getIntfieldValueWithName no pointer");
	}

	return ret;
}

JNIEXPORT jfloat JNICALL easysqlite_getFloatFieldValueWithName
  (JNIEnv *env, jobject obj, jint pointer, jstring name)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	jfloat ret = 0;
	const char *charname = env->GetStringUTFChars(name, 0);
	if(t)
	{
		try
		{
			ret = t->getFloatField(charname);
			LOGD("easysqlite_getIntfieldValueWithName sql: %f", ret);
			
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_getIntfieldValueWithName no pointer");
	}

	env->ReleaseStringUTFChars(name, charname);

	return ret;
}


JNIEXPORT jfloat JNICALL easysqlite_getFloatFieldValueWithIndex
  (JNIEnv *env, jobject obj, jint pointer, jint index)
{
	CppSQLite3Query* t = getQueryResultItem(pointer);
	jfloat ret = 0;
	if(t)
	{
		try
		{
			ret = t->getFloatField(index);
			LOGD("easysqlite_getIntfieldValueWithName sql: %f", ret);
		}
		catch (CppSQLite3Exception& e)
		{
			LOGD("errorCode:%d,errorMessage:%s",e.errorCode(),e.errorMessage());
		}
	}
	else
	{
		LOGD("easysqlite_getIntfieldValueWithName no pointer");
	}

	return ret;
}


/***********************************美丽的分隔线**********************************************/


static JNINativeMethod sMethods[] =
{
    // name, signature, funcPtr
    {"getSQLiteVersion", "()Ljava/lang/String;", (void *)easysqlite_version},
    {"open", "(Ljava/lang/String;)I",(void *)easysqlite_open},
    {"openWithPassword", "(Ljava/lang/String;Ljava/lang/String;)I",(void *)easysqlite_openWithPassword},
    {"close", "()V",(void *)easysqlite_close},
    {"clear", "()V",(void *)easysqlite_clear},
    {"execDML", "(Ljava/lang/String;)I",(void *)easysqlite_execDML},
    {"execQuery", "(Ljava/lang/String;)I",(void *)easysqlite_execQuery},
    {"clearKey", "(Ljava/lang/String;)Z",(void *)easysqlite_clearKey},
    {"setKey", "(Ljava/lang/String;)Z",(void *)easysqlite_setKey},
    {"changeKey", "(Ljava/lang/String;Ljava/lang/String;)Z",(void *)easysqlite_changeKey},
    {"eof", "(I)Z",(void *)easysqlite_eof},
    {"nextRow", "(I)V",(void *)easysqlite_nextRow},
    {"fieldValueWithName", "(ILjava/lang/String;)Ljava/lang/String;",(void *)easysqlite_fieldValueWithName},
    {"fieldValueWithIndex", "(II)Ljava/lang/String;",(void *)easysqlite_fieldValueWithIndex},
    {"getStringFieldValueWithName", "(ILjava/lang/String;)Ljava/lang/String;",(void *)easysqlite_getStringFieldValueWithName},
    {"getStringFieldValueWithIndex", "(II)Ljava/lang/String;",(void *)easysqlite_getStringFieldValueWithIndex},
    {"getIntFieldValueWithName", "(ILjava/lang/String;)I",(void *)easysqlite_getIntFieldValueWithName},
    {"getIntFieldValueWithIndex", "(II)I",(void *)easysqlite_getIntFieldValueWithIndex},
    {"getFloatFieldValueWithName", "(ILjava/lang/String;)F",(void *)easysqlite_getFloatFieldValueWithName},
    {"getFloatFieldValueWithIndex", "(II)F",(void *)easysqlite_getFloatFieldValueWithIndex},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, JNIREG_CLASS, sMethods, sizeof(sMethods) / sizeof(sMethods[0])))
	{
		LOGI("registerNatives false ");
		return JNI_FALSE;
	}
	LOGI("registerNatives true ");
	return JNI_TRUE;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{ 
	JNIEnv *env;
	//gJavaVM = vm;
	LOGI("JNI_OnLoad called");
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_2) != JNI_OK) {
	LOGE("Failed to get the environment using GetEnv()");
	return -1;
	}
	
	LOGI("JNI_OnLoad register methods ");
	registerNatives(env);

	
	return JNI_VERSION_1_2;
} 

extern "C" void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    JNIEnv *env;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_2)) {
		return;
    }

	clearQueryResultItem();
	deleteSQLite();
}

