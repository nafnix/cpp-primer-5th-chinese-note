#include<string>
#include<set>

class Folder;

// Message类
class Message {
    friend class Folder;

public:
    // folders被隐式初始化为空集合
    explicit Message(const std::string &str = ""): contents(str) { }// 默认消息文本设为空字符串

    // 拷贝控制成员 用于管理指向本Message的指针
    Message(const Message&);                // 拷贝构造函数
    Message& operator=(const Message&);     // 拷贝赋值运算符
    ~Message();                             // 析构函数

    // 从给定Folder集合里添加/删除本Message
    void save(Folder&);
    void remove(Folder&);

private:
    std::string contents;                   // 实际消息文本
    std::set<Folder*> folders;              // 包含本Message的Folder集合 隐式初始化为空集

    // 拷贝构造函数、拷贝赋值运算符和析构函数所使用的工具函数
    void add_to_Folders(const Message&);    // 将本Message添加到指向参数的Folder中

    void remove_from_Folders();             // 从folders中的每个Folder中删除本Message
};

void Message::save(Folder &f)
{
    folders.insert(&f);     // 将给定的Folder添加到我们的Folder列表里
    f.addMsg(this);         // 将本Message添加到f的Message集合里
}

void Message::remove(Folder &f)
{
    folders.erase(&f);      // 将给定Folder从我们的Folder列表中删除
    f.remMsg(this);         // 将本Message从f的Message集合中删除
}

void Message::add_to_Folders(const Message &m)          // 接受一个Message对象作为参数m
{
    for (auto f : m.folders)                            // 对于m所在的每个Folder对象
        f->addMsg(this);                                // 将本Message添加到里面
}

Message::Message(const Message &m): contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);
}

void Message::remove_from_Folders()
{
    for (auto f : folders)      // 使用f遍历本Message所在的folders集合
        f->remMsg(this);        // 对f调用remMsg操作，并且将本Message传入进去
}

Message::~Message()
{
    remove_from_Folders();
}

Message& Message::operator=(const Message &rhs)     // 接受一个Message对象rhs作为新值
{
    remove_from_Folders();          // 从左值所在的Folder中删除左值
    contents = rhs.contents;        // 从赋予的新对象中拷贝contents消息内容
    folders = rhs.folders;          // 从赋予的新对象中拷贝folders集合
    add_to_Folders(rhs);            // 将本Message添加到赋予的新对象中的folders集合里
    return *this;                   // 返回本Message
}

void swap(Message &lhs, Message &rhs)
{
    using std::swap;        // 本例中其实可以不用 但这是个好习惯
    
    // 将每个消息从它所在的Folder中删除
    for (auto f: lhs.folders)
        f->remMsg(&lhs);
    for (auto f: rhs.folders)
        f->remMsg(&rhs);
    
    // 交换contents和Folder指针集合
    swap(lhs.folders, rhs.folders);         // 使用swap替换所在的消息集合
    swap(lhs.contents, rhs.contents);       // 使用swap替换消息内容

    // 将每个Message对象添加到它的新Folder中
    for (auto f: lhs.folders)
        f->addMsg(&lhs);
    for (auto f: rhs.folders)
        f->addMsg(&rhs);
}


// 从本Message移动Folder指针
void Message::move_Folders(Message *m)
{
    folders = std::move(m->folders);        // 使用set的移动赋值运算符
    for (auto f : folders) {                // 对每个Folder
        f->remMsg(m);                       // 从Folder中删除旧Message
        f->addmsg(this);                    // 将本Message添加到Folder中
    }
    m->folders.clear();                     // 确保销毁m无害
}


Message::Message(Message &&m): contents(std::move(m.contents))
{
    move_Folders(&m);       // 移动folders并且更新Folder指针
}


Message& Message::operator=(Message &&rhs)
{
    if (this != &rhs) {     // 直接检查自赋值情况
        remove_from_Folders();
        contents = std::move(rhs.contents); // 移动赋值运算符
        move_Folders(&rhs);                 // 重置Folders指向本Message 
    }
    return *this;
}