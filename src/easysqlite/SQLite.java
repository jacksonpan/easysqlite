package easysqlite;

import java.io.File;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

public class SQLite {
	private static SQLite single = null;
	private static SQLiteCore sqlCore = null;
	public static Context ctx;

	public synchronized static SQLite getInstance(Context context) {
		ctx = context;
		if (single == null) {
			single = new SQLite();
		}
		return single;
	}

	private SQLite() {
		sqlCore = SQLiteCore.getInstance();
	}

	public static int SQLITE_OK = 0;
	public static int SQLITE_ERROR = 1;

	public static String getSQLiteVersion()
	{
		return SQLiteCore.getSQLiteVersion();
	}

	public int openDatabase(String dbName) {
		File databasePath = ctx.getDatabasePath(dbName);
		if (!databasePath.exists()) {
			SQLiteHelper help = new SQLiteHelper(ctx, dbName);
			help.getReadableDatabase();
			help.close();
		}
		return sqlCore.open(databasePath.getPath());
	}

	public int openDatabase(String dbName, String dbPassword) {
		File databasePath = ctx.getDatabasePath(dbName);
		if (!databasePath.exists()) {
			SQLiteHelper help = new SQLiteHelper(ctx, dbName);
			help.getReadableDatabase();
			help.close();
			
			if (dbPassword.length() == 0) {
				return sqlCore.open(databasePath.getPath());
			} else {
				sqlCore.open(databasePath.getPath());
				sqlCore.setKey(dbPassword);
				sqlCore.close();
				return sqlCore.openWithPassword(databasePath.getPath(), dbPassword);
			}
		}
		else
		{
			if (dbPassword.length() == 0) {
				return sqlCore.open(databasePath.getPath());
			} else {
				return sqlCore.openWithPassword(databasePath.getPath(), dbPassword);
			}
		}
	}

	public void closeDatabase() {
		sqlCore.close();
	}

	public void clearDatabase() {
		sqlCore.clear();
	}

	public int executeDML(String sql) {
		return sqlCore.execDML(sql);
	}
	
	public SQLiteQuery executeQuery(String sql){
		int ret = sqlCore.execQuery(sql);
		if(ret == SQLITE_ERROR)
		{
			return null;
		}
		SQLiteQuery query = new SQLiteQuery(ret);
		return query;
	}

	public boolean setPassword(String key) {
		return sqlCore.setKey(key);
	}

	public boolean clearPassword(String key) {
		return sqlCore.clearKey(key);
	}

	public boolean changePassword(String oldKey, String newKey) {
		return sqlCore.changeKey(oldKey, newKey);
	}
}
