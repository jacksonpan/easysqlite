package com.easysqlite_test;

import easysqlite.SQLite;
import easysqlite.SQLiteQuery;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		MyApplication.setMainActivity(this);
		
		String version = SQLite.getSQLiteVersion();
		System.out.println(version);
		
		SQLite sqliteHelper = SQLite.getInstance(MyApplication.getMainActivity());
		String dbName = "test.db";
		int ret = sqliteHelper.openDatabase(dbName, "123456");
		//boolean ret2 = sqliteHelper.setPassword("123456");
		//ret2 = sqliteHelper.clearPassword("123456");
		ret = sqliteHelper.executeDML("CREATE TABLE person (_id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, age INTEGER)");
	    ret = sqliteHelper.executeDML("INSERT INTO person VALUES (NULL, 'jack1', 21)");
	    ret = sqliteHelper.executeDML("INSERT INTO person VALUES (NULL, 'jack2', 22)");
	    ret = sqliteHelper.executeDML("INSERT INTO person VALUES (NULL, 'jack3', 23)");
	    ret = sqliteHelper.executeDML("INSERT INTO person VALUES (NULL, 'jack4', 24)");
	    
		SQLiteQuery query = sqliteHelper.executeQuery("select * from person");
		while(!query.eof())
		{
			String name = query.getStringFieldValue("name");
			int age = query.getIntFieldValue("age");
			float _id = query.getFloatFieldValue("_id");
			System.out.println(name);
			System.out.println(age);
			System.out.println(_id);
			query.nextRow();
		}
		
		
		sqliteHelper.closeDatabase();
	    sqliteHelper.clearDatabase();
		
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
}
