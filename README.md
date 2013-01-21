easysqlite是为了iOS、android以及更多需要使用sqlite数据库的开发者专门提供的简单使用sqlite的开发库，该库提供了AES256对sqlite数据库加密以及解密，使用方法非常简单。

easysqlite使用到了开源项目wxsqlite3（http://wxcode.sourceforge.net/docs/wxsqlite3/）的sqlite部分和CppSQLite3（https://github.com/NeoSmart/CppSQLite），在前者基础上增加了编译宏，在后者基础上增加了sqlite加密和解密的封装。

easysqlite自己开发了针对android使用的sqlite_jni封装，详情看easysqlite_jni.cpp，android通过so库调用easysqlite_jni的接口，由于是刚完成的，目前还有很多不完善，正在努力完善中。。。

easysqlite在iOS中的使用就比较简单，直接编译sqlite3secure.c就可以了,object-c封装推荐使用FMDatabase。

本人QQ:530933208，欢迎爱好者一起来维护这个项目为更好的使用sqlite，更简单的使用加解密，更方便的让初学者简单编译使用，一起努力吧！

License:
follow GPL license