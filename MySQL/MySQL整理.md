select @@have_profiling; 



explain select*from t_customer where name:='zhangsan'and age:=20 and gender='M';/*完全使用了索引，索引长度1033个字节*/
explain select*from t_customer where name=:'zhangsan'and age=20;/*部分使用了索引，索引长度1828，说明gender2在索引当中占用5个字节的长度*/
explain select*from t_customer where name='zhangsan';/*部分使用了索引，索引长度1023，说明age在索引当中占用5个字节的长度*/
explain select*from t_customer where age=:20 and gender='M'and name='zhangsan';/*完全使用了索引，索引长度1o33个字节*/
explain select*from t._customer where gender='M'and age:=26;/*因为条件不符合最左前缀原则，没有name条件出现，所以不会使用索引，进行全表扫描*/
explain select*from t_customer where name='zhangsan'and gender='M';/*部分使用了素引，只有name字段使用素引，因为中间断开了，导致gender没有使用素引*/
explain select*from t_customer where name='zhangsan'and gender='M'and age=20;/*完全使用了索引*/
explain select*from t_customer where name=:'zhangsan'and age>2 3 and gender='M';/*当使用范围查找的时候，范围条件右侧的列不会使用素引。*/
explain select*from t._customer where name=:'zhangsan'and age>=20 and gender='M';/*建议范围条件中添加等号，这样可以让索引完全生效*/



1.索引列参加了算术运算 索引失效 
假设salary列有索引
```mysql
select * from table where salary >1000;

select * from table where salary*10 >1000;  失效！！！！
```

1.索引列参加了模糊查询 索引失效 