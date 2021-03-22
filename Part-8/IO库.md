# IO库

C++语言不直接处理输入输出，而是通过一个定义在标准库里的类型来处理IO。

这些类型支持从设备读取数据、向设备写入数据的IO操作，设备可以是文件、控制台窗口等。

还有些类型允许内存IO，即，从string读取数据，向string写入数据。

IO库定义了读写内置类型值的操作。此外，一些类，如string，通常也会定义类似的IO操作，来读取自己的对象。

已经用过的IO库设施：

- istream类型，提供输入操作
- ostream类型，提供输出操作
- cin，istream对象，从标准输入读取数据
- cout，ostream对象，从标准输出写入数据
- cerr，ostream对象，通常用于输出程序错误消息，写入到标准错误
- `>>`运算符，用以从一个istream对象读取输入数据
- `<<`运算符，用以从一个ostream对象写入输出数据
- getline函数，从一个给定的istream读取一行数据，存入一个给定的string对象中

## IO类

目前用过的IO类型和对象都是操纵`char`数据的。默认这些对象都是关联到用户的控制台窗口。

为支持不同种类的IO处理操作，标准库除了istream和ostream还定义了其他的IO类型。

分别定义在三个独立的头文件中：

| 头文件   | 类型                                          |
| -------- | --------------------------------------------- |
| iostream | istream, wistream从流读取数据                 |
|          | ostream, wostream向流写入数据                 |
|          | iostream, wiostream读写流                     |
|          |                                               |
| fstream  | ifstream, wifstream从文件读取数据             |
|          | ofstream, wofstream从文件写入数据             |
|          | fstream, wfstream读写文件                     |
|          |                                               |
| sstream  | istringstream, wistringstream从string读取数据 |
|          | ostringstream,wostringstream向string写入数据  |
|          | stringstream, wstringstream读写string         |

`sstream`用于读写内存`string`对象的类型。

为支持使用宽字符语言，标准库定义了一组类型和对象来操纵`wchar_t`类型的数据。宽字符版本的类型和函数名以`w`开始。如`wcin`、`wcout`和`wcerr`。

#### IO类型间的关系

通过__继承机制__(inheritance)可以使我们忽略读取的字符要存进`char`对象还是`wchar_t`对象。

继承机制使我们可以声明一个特定的类继承自另一个类。通常可以将一个派生类(继承类)对象当作其基类(所继承的类)对象来使用。

`ifstream`和`istringstream`都继承自`istream`。所以可以像用`istream`对象一样用`ifstream`和`istringstream`对象。

### IO对象无拷贝或赋值

不能拷贝或者对IO对象赋值

```cpp
ofstream out1, out2;
out1 = out2;				// 错误 不能对流对象赋值
ofstream print(ofstream);	// 错误 不能初始化ofstream参数
out2 = print(out2);			// 错误 不能拷贝流对象
```

因为不能拷贝IO对象，所以也不能将形参或者返回类型设置为流类型。

进行IO操作的函数通常以引用方式传递和返回流。

读写一个IO对象会改变其状态，因此传递和返回的引用不能是`cosnt`的。

### 条件状态

IO操作一个问题就是可能发生错误。一些错误是可以恢复的，而其他粗我则发生在系统深处，已经超出了应用程序可以修正的范围。

| IO类定义的一些函数和标志 | IO条件状态                                                   |
| ------------------------ | ------------------------------------------------------------ |
| `strm::iostate`          | `strm`是种IO类型，如`istream`。<br />`iostate`是种机器相关的**类型**，提供了表达条件状态的完整功能 |
| `strm::badbit`           | 用来指出流已经崩溃                                           |
| `strm::failbit`          | 用来指出一个IO操作失败了                                     |
| `strm::eofbit`           | 用来指出流到达了文件结束                                     |
| `strm::goodbit`          | 用来指出流未处于错误状态。此值保证为0                        |
| `s.eof()`                | 若流`s`的`eofbit`置位，则返回true                            |
| `s.fail()`               | 若流`s`的`failbit`或`badbit`置位，则返回true                 |
| `s.bad()`                | 若流`s`的`badbit`置位，则返回true                            |
| `s.good()`               | 若流`s`处于有效状态，则返回true                              |
| `s.clear()`              | 将流`s`中所有条件状态位复位，将流的状态设置为有效。返回void  |
| `s.clar(flags)`          | 根据给定的`flags`标志位，将流`s`中对应条件状态位复位。<br />`flags`的类型为`strm::iostate`。返回void |
| `s.setstate(flags)`      | 根据给定的`flags`标志位，将流`s`中对应条件状态位置位。<br />`flags`的类型为`strm::iostate`。返回void |
| `s.rdstate()`            | 返回流`s`的当前条件状态，返回值类型为`strm::iostate`         |

上述表可以帮助我们访问和操纵流的**条件状态**(condition state)。

IO错误示例：

```cpp
int ival;
cin >> ival;
```

若我们在标准输入上输入`Boo`，读操作就会失败。因为`ival`是期望接收`int`对象。

一个流一旦发生错误，后续的IO操作都会失败。只有当一个流处于无错状态时候，我们才能对它读取数据或写入数据。

由于流可能是在错误状态，因此最好是在使用流前判断它的状态：

```cpp
while(cin >> word)
	;// 读操作成功
```

#### 查询流的状态

将流作为条件只能告诉我们流是否有效，但是有时候我们需要知道到底发生了什么，为什么流会失败。例如在输入文件结束标识后我们的应对措施可能与遇到一个IO设备错误的处理方式不同。

`iostate`类型与机器无关，它提供了表达流状态的完整功能。这个类型应该作为一个位集合来使用，[使用方式与之类似](https://github.com/nafnix/C-Primer5th-Note-chinese/blob/master/Part%204%20%E8%A1%A8%E8%BE%BE%E5%BC%8F.md#%E4%BD%8D%E8%BF%90%E7%AE%97%E7%AC%A6)。IO库定义了4个`iostate`类型的`constexpr`值，表示特定的位模式。这些值用来表示特定类型的IO条件，可以与位运算符一起使用来一次性检测或设置多个标志位。

`batbit`表示系统级错误，比如不可恢复的读写操作。通常`batbit`被置位，流就无法再使用了。

在发生可恢复错误后，`failbit`被置位，如期望读取数值却读出一个字符等错误。这种问题通常是可以被修正的，流还可以继续使用。

如果到达文件结束位置，`eofbit`和`failbit`都会被置位。`goodbit`的值为0，表示流还没发生错误。如果`badbit`、`failbit`、`eofbit`任何一个被置位，则检测流状态的条件都会失败。

#### 其它的函数

`good`在所有错误位均为置位的情况下返回true

`bad`、`fail`、`eof`在对应错误被置位时返回true

`badbit`被置位的时候，`fail`也会返回true。也就是使用`good`或`fail`是确定流的总体状态的正确方式。

事实上只要把流当作条件使用的代码就相当于`!fail()`。

`eof`和`bad`只能表示特定的错误。

#### 管理条件状态

流对象的`rdstate`成员返回`iostate`值，对应流的当前状态。

`setstate`将给定条件位置位，表示发生了对应错误。

`clear`成员是个重载成员，可以接受也可以不接受一个`iostate`类型的参数：

- 若没有参数，则清除(复位)所有错误标志位。

  - 执行之后，若再调用`good`就会返回true

  - ```cpp
    int main(){
        // 记住cin的当前状态
        auto old_state = cin.rdstate();		// 记住cin的当前状态
        cin.clear();						// 使cin有效
        process_input(cin);					// 使用cin
        cin.setstate(old_state);			// 使cin置为原有状态
    }
    ```

- 若有参数，参数表示流的新状态。为了复位单一的条件状态位，先用`rdstate`读出当前条件状态，然后用位操作将所需位复位来生产新的状态。

  - 示例将`falibit`和`badbit`复位，但是保持`eofbit`不变

  - ```cpp
    int main(){
        cin.clear(
        	cin.rdstate() & ~cin.failbit & ~cin.badbit
        );
    }
    ```

### 管理输出缓冲

每个输出流都管理一个缓冲区，用来保存程序读写的数据。

```cpp
os << "please enter a value: ";
```

文本串可能被立即打印出来，但也可能被操作系统保存在缓冲区之后再打印出来。有了缓冲机制，操作系统就可以把程序的多个输出操作组合成单一的系统级写操作。将多个输出操作合成一个的操作会有性能提升。

导致缓冲刷新，也就是数据真正写到输出设备或者文件的原因有：

- 程序正常结束，作为`main`函数的`return`操作的一部分，缓冲刷新被立即执行。
- 缓冲区满时，需要刷新缓冲，不然后面的数据写不进缓冲区。
- 可以使用操作符如`endl`来显式刷新缓冲区。
- 输出操作之后，可以用操作符`unitbuf`设置流的内部状态，来清空缓冲区。默认`cerr`设置`unitbuf`。
- 一个输出流可能被关联到另一个流。这时如果读写被关联的流，那个流的缓冲区就会被刷新。比如默认`cin`和`cerr`都关联到`cout`。所以读`cin`或写`cerr`都会导致`cout`的缓冲区被刷新。

#### 刷新输出缓冲区

与`endl`类似的操作符：

- `flush`：刷新缓冲区，不输出任何额外的字符
- `ends`：向缓冲区插入一个空字符然后刷新缓冲区

```cpp
cout << "hi!" << endl;		// 输出hi! 和一个换行	 然后刷新缓冲区
cout << "hi!" << flush;		// 输出hi!		    然后刷新缓冲区
cout << "hi!" << ends;		// 输出hi! 和一个空字符 然后刷新缓冲区 
```

#### unitbuf操作符

使用`unitbuf`操作符在每次输出就刷新缓冲区。

使用`nounitbuf`操作符重置流，使其恢复使用正常的系统管理额缓冲区刷新机制。

如果程序异常终止，输出缓冲区是不会被刷新的。

#### 关联输入和输出流

如果输入流被关联到输出流，那么输入流读取数据时会先刷新关联的输出流。

标准库将`cout`和`cin`关联到一起，所以`cin>>ival;`导致`cout`的缓冲区被刷新。

交互式系统通常应该关联输入流和输出流。这意味着所有输出，包括用户提示信息，都会在读操作之前被打印出来。

`tie`有俩版本：

- 若没有参数，返回指向输出流的指针。
  - 若本对象当前关联到一个输出流，则返回的就是指向这个流的指针。
  - 如果没关联到流，则返回空指针。
- 参数接受一个指向`ostream`的指针，将自己关联到此`ostream`。`x.tie(&o)`将流`x`关联到输出流`o`

可以将输入或者输出流关联到输出流：

```cpp
cin.tie(&cout);							// 仅用于展示 
ostream *old_tie = cin.tie(nullptr);	// 将old_tie指向当前关联到cin的流(若有) cin流不再与其他流关联

cin.tie(&cerr);							// 将cin与cerr关联 可以 但是cin应该关联cout 读取cin会刷新cerr而不是cout

cin.tie(old_tie);						// 重建cin和cout间的正常关联
```

`cin.tie(nullptr)`表示解开`cin`和其他流的关联。

每个流只能关联一个流，但多个流可以关联同一个流。

## 文件输入输出

头文件`fstream`定义了三个类型来支持文件IO：

- **ifstream**：从给定文件读取数据
- **ofstream**：向给定文件写入数据
- **fstream**：读写给定文件

管理与流关联的文件：

| fstream特有的操作         |                                                              |
| ------------------------- | ------------------------------------------------------------ |
| `fstream strm;`           | 创建一个未绑定的文件流。`fstream`是`fstream`头文件里定义的一个类型 |
| `fstream fstrm(s);`       | 创建文件流，打开名为`s`的文件。<br />`s`可以是`string`或者指向C风格字符串的指针。<br />这些构造函数都是`explicit`必须接受一个参数。<br />默认的文件模式`mode`依赖于`fstream`类型 |
| `fstream fstrm(s, mode);` | 与前一个构造函数类似，但按指定`mode`打开文件                 |
| `fstrm.open(s)`           | 打开名为`s`的文件，并将文件与`fstrm`绑定。<br />`s`可以是`string`或者C风格字符串。<br />默认的`mode`依赖于`fstream`类型。<br />返回`void` |
| `fstrm.close()`           | 关闭`fstrm`绑定的文件。返回`void`                            |
| `fstrm.is_open()`         | 返回一个`bool`值，指出`fstrm`关联的文件是否处于成功打开状态。 |

### 使用文件流对象

创建文件流对象，文件名参数可选。若提供文件名，则`open`会被自动调用：

```cpp
#include<fstream>
ifstream in(ifile);			// 构造一个ifstream并打开给定文件 定义一个输入流in
ofstream out;				// 输出文件流未关联到任何文件 定义输出流out
```

#### 用fstream替代iostream&

要求使用基类型对象的地方，可以继承类型的对象来替代。也就是说接受一个`iostream`类型引用参数，同时也可以接受对应的`fstream`或者`sstream`。

##### 文件输出流示例

```cpp
int main(){
    string filename = "print.txt";
    ofstream outputfile;                    // 创建输出流
    outputfile.open(filename);              // 输出流读入文件
    if (outputfile){
        outputfile << "测试输出流是否可以成功输出" << endl;
    }
    outputfile.close();
    return 0;
}
```

##### 文件写入流示例

```cpp
int main(){
    string filename = "print.txt";
    string data{"用于存放写入的数据"};
    ifstream inputfile;                     // 创建输出流
    inputfile.open(filename);              // 输入流读入文件
    if (inputfile){
        inputfile >> data;                  // 从文件中读入数据 会覆盖掉原有的data中的内容
    }
    inputfile.close();
    cout << data << endl;                   // 显示从文件中读入的内容
    return 0;
}
```

#### 成员函数open和close

如果定义了一个空文件流对象，可以随后调用`open`来将它与文件关联起来。

```cpp
    string file_name = "print.txt";
    ifstream test(file_name);
    ofstream out;
    if(out) out.open(file_name);
```

若调用`open`失败，`failbit`会被置位。所以我们最好先检查`open`是否成功：

```cpp
if (out)
```

对一个已经打开的文件流调用`open`会失败，并会导致`failbit`被置位。

如果想把文件流关联到另一个文件，必须先关闭已经关联的文件。

```cpp
in.close();
in.open("print2.txt");
```

若`open`成功，则`open`会将流的状态的`good()`设置为`true`。

### 文件模式

每个流都有一个关联的**文件模式**(file mode)。

- in：以读方式打开
- out：以写方式打开
- app：每次写操作前均定位到文件末尾
- ate：打开文件后立即定位到文件末尾
- trunc：截断文件
- binary：以二进制方式进行IO

无论哪种方式打开都可以指定文件模式。

指定文件模式的限制：

- 只能对`ofstream`或`fstream`设定`out`模式
- 只能对`ifstream`或`fstream`设定`in`模式
- `trunc`模式的前提是设定`out`
- `trunc`没被设定就可以设定`app`模式。如果设定了`app`那么也可以不设定`out`。
- 默认如果没有指定`trunc`，那么以`out`模式打开文件也会被阶段。
- `ate`和`binary`模式可用于任何类型的文件流对象，且可以与其他任何文件模式组合使用。

每个文件流都定义了默认的文件模式。`ifstream`默认`in`，`ofstream`默认`out`，`fstream`默认以`in`和`out`。

只使用`out`模式打开文件会丢弃已有数据。

每次调用`open`时候都会确定文件模式。

```cpp
ofstream out;			// 没有指定文件打开模式
out.open("file");		// 模式隐含设置为输出和截断
out.close();			// 关闭out 是我们可以将其用于其他文件
out.open("file", ofstream::app);	// 模式为输出和追加
out.close();
```

## string流

- `istringstream`：从string读取数据
- `ostringstream`：向string写入数据
- `stringstream`：既可以从string读数据也可以向string写数据。

头文件`sstream`中定义的类型继承自`iostream`头文件中定义的类型。

除了继承得来的操作，还有其他的一些成员：

- `sstream strm;`：`strm`是个未绑定的`stringstream`对象。`sstream`是头文件sstream中定义的一个类型
- `sstrean strn(s)`：`strm`是个`sstream`对象，保存`string`类型的`s`的一个拷贝。此构造函数是`explicit`
- `strm.str()`：返回`strm`保存的`string`的拷贝
- `strm.str(s)`：将`string`类型的`s`拷贝到`strm`中。返回`void`

### 使用istringstream

有时我们的工作是对整行文本惊醒处理，而其他一些工作是处理了行内的单个单词时，就可以用`istringstream`。

比如，有个文件，该文件有一些人以及他们的电话号码。某些人只有一个号码，而有些人有多个号码。那么我们输入文件看起来可能是这样的：

```
morgan 2015552368 8625550123
drew 9735550130
lee 6095550132 2015550175 8005550000
```

每条记录都是一个人名开始，后面跟着的都是电话

那么可以定义一个简单的类来描述输入数据：

```cpp
struct PersionInfo{
	std::string name;
    std::vector<std::string> phones;
};
```

类型`PerionInfo`对象会有一个成员来表示任命，还有一个`vector`来保存这个人的所有电话

使用示例：

```cpp
#include<sstream>
using namespace::std;

int main(){
    string line, word;                      // 分别保存来自输入的一行和单词
    vector<PersionInfo> people;             // 保存来自输入的所有记录
    // 逐行从输入读取数据，直至cin遇到文件尾(或其他错误)
    while (getline(cin, line)){
        PersionInfo info;                       // 创建一个保存此记录数据的对象
        istringstream record(line);             // 将记录绑定到刚读入的行
        record >> info.name;                    // 读取名字
        while (record >> word)                  // 读取电话号码
            info.phones.push_back(word);            // 保存他们
        people.push_back(info);                 // 将该记录追加到people末尾
    }
    return 0;
}
```

### 使用ostringstream

```cpp
    for (const auto &entry : people) {          // 对people中每一项
        ostringstream formatted, badNums;           //每个循环步创建的对象
        for (const auto &nums : entry.phones){      // 对每个数
            if (!valid(nums)){
                badNums << " " << nums;                 // 将数的字符串形式存入badNums
            }else
                // 将格式化的字符写入formatted
                formatted << " " << format(nums);
        }
        if (badNums.str().empty)            // 如果没有错误的数
            os << entry.name << " " << formatted.str() << endl; // 打印名字和格式化的数
        else
            cerr << "输入错误:" << entry.name << "数" << badNums.str() << endl;
    }
```

此例程序中，我们假定已有`valid`和`format`函数，分别完成电话号码验证和改变格式的功能。