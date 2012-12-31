package com.easysqlite_test;

import android.app.Application;

public class MyApplication extends Application {
	private static MyApplication instance;
	private static MainActivity instance2;

	public static MyApplication getInstance() {
		return instance;
	}

	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		instance = this;
	}

	/**
	 * @return the instance2
	 */
	public static MainActivity getMainActivity() {
		return instance2;
	}

	/**
	 * @param instance2 the instance2 to set
	 */
	public static void setMainActivity(MainActivity instance2) {
		MyApplication.instance2 = instance2;
	}
	
}