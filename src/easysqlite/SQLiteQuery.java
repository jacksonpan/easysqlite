package easysqlite;

public class SQLiteQuery {
	private static int pointerCppSQLite3Query;
	private static SQLiteCore sqlCore = null;
	
	public SQLiteQuery(int pointer)
	{
		pointerCppSQLite3Query = pointer;
		sqlCore = SQLiteCore.getInstance();
	}
	
	public boolean eof()
	{
		return sqlCore.eof(pointerCppSQLite3Query);
	}
	
	public void nextRow()
	{
		sqlCore.nextRow(pointerCppSQLite3Query);
	}
	
	public String fieldValue(String name)
	{
		return sqlCore.fieldValueWithName(pointerCppSQLite3Query, name);
	}
	
	public String fieldValue(int index)
	{
		return sqlCore.fieldValueWithIndex(pointerCppSQLite3Query, index);
	}
	
	public String getStringFieldValue(String name)
	{
		return sqlCore.getStringFieldValueWithName(pointerCppSQLite3Query, name);
	}
	
	public String getStringFieldValue(int index)
	{
		return sqlCore.getStringFieldValueWithIndex(pointerCppSQLite3Query, index);
	}
	
	public int getIntFieldValue(String name)
	{
		return sqlCore.getIntFieldValueWithName(pointerCppSQLite3Query, name);
	}
	
	public int getIntFieldValue(int index)
	{
		return sqlCore.getIntFieldValueWithIndex(pointerCppSQLite3Query, index);
	}
	
	public float getFloatFieldValue(String name)
	{
		return sqlCore.getFloatFieldValueWithName(pointerCppSQLite3Query, name);
	}
	
	public float getFloatFieldValue(int index)
	{
		return sqlCore.getFloatFieldValueWithIndex(pointerCppSQLite3Query, index);
	}
}
