/// @brief //#内容详细请看c++官方文档
int main(int argc, char const* argv[])
{
  //*关键词只有在属性记号中可做他用 如:[[int]]
  [[maybe_unused]] int x = 0;
  [[maybe_unused]] int a[0]{};

  decltype(x) y1;
  //! decltype((x))y2 = 0;    //非常量引用只能绑定左值
  decltype((x)) y2 = y1;  //*成立 y2为可变左值引用
  decltype(+(x)) y3;      //*成立 y3等同y1
  //! decltype(a) y4 = {0} ;  //类型为int类型0长数组只能赋值为空
  decltype(a) y4 = {};   //*成立 y4为int类型0长数组
  decltype(+a) y5;       //*成立 y5为int类型指针
  decltype(*a) y6 = y1;  //*成立 y6为int引用同(a[0])
  decltype(+*a) y7;      //*成立 y7为int类型
  
  return 0;
}
