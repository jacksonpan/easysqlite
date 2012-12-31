package easysqlite;

public class SQLiteCore {
	private static SQLiteCore single = null;

	public synchronized static SQLiteCore getInstance() {
		if (single == null) {
			single = new SQLiteCore();
		}
		return single;
	}
	
	private SQLiteCore(){
		
	}
	
	public native static String getSQLiteVersion();
	
	/*
	 * return SQLITE_OK (success) or Other (fail);
	 */
	public native int open(String dbPath);
	public native int openWithPassword(String dbPath, String dbPassword);
	public native void close();
	
	/*
	 * clear memory for sqlite, if you want use again, please open database again
	 */
	public native void clear();
	
	/*
	 * return SQLITE_OK (success) or Other (fail);
	 */
	public native int execDML(String sql);
	
	/*
	 * return not SQLITE_ERROR is success, the result 'int' is an CppSQLite3Query pointer
	 */
	public native int execQuery(String sql);
	
	public native boolean clearKey(String key);
	public native boolean setKey(String key);
	public native boolean changeKey(String oldKey, String newKey);
	
	/*
	 * for query
	 */
	public native boolean eof(int pointer);
	public native void nextRow(int pointer);
	public native String fieldValueWithName(int pointer, String name);
	public native String fieldValueWithIndex(int pointer, int index);
	public native String getStringFieldValueWithName(int pointer, String name);
	public native String getStringFieldValueWithIndex(int pointer, int index);
	public native int getIntFieldValueWithName(int pointer, String name);
	public native int getIntFieldValueWithIndex(int pointer, int index);
	public native float getFloatFieldValueWithName(int pointer, String name);
	public native float getFloatFieldValueWithIndex(int pointer, int index);
	
	static{
		System.loadLibrary("easysqlite_jni");
	}
}
