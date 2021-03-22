#include<memory>
#include<string>
#include<vector>
#include<initializer_list>
#include<iostream>

class StrBlobPtr;       // 前置声明
class StrBlob {
friend class StrBlobPtr;
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    // 添加或者删除元素
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    // 元素访问
    std::string& front();
    std::string& back();
private:
    std::shared_ptr<std::vector<std::string>> data;
    // 若data[i]不合法 抛出一个异常
    void check(size_type i, const std::string &msg) const;
};

StrBlob::StrBlob(): data(std::make_shared<std::vector<std::string>>()) { }
StrBlob::StrBlob(std::initializer_list<std::string> il): data(std::make_shared<std::vector<std::string>>(il)) { }

#include<stdexcept>
void StrBlob::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}

std::string& StrBlob::front()
{
    // 若vector为空 check会抛出一个异常
    check(0, "front on empty StrBlob");
    return data->front();
}

std::string& StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}


// 对于访问一个不存在元素的尝试 StrBlobPtr抛出一个异常
class StrBlobPtr {
public:
    StrBlobPtr(): curr(0) { }
    StrBlobPtr(StrBlob &a, size_t sz = 0):
        wptr(a.data), curr(sz) { }
    std::string& deref() const;
    StrBlobPtr& incr();     // 前缀递增

private:
    // 如果检查成功 check返回一个指向vector的shared_ptr
    std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
    // 保存一个weak_ptr 意味着底层的vector可能会被销毁
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};

std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i, const std::string &msg) const
{
    auto ret = wptr.lock();     // vector是否还在
    if (!ret)                   // 如果不在
        throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())       // 判断ret的引用计数是否大于i
        throw std::out_of_range(msg);
    return ret;
}

std::string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*p)是对象所指向的vector
}

// 前缀递增 返回递增后的对象的引用
StrBlobPtr& StrBlobPtr::incr()
{
    // 如果curr已经指向容器的尾后位置 就不能递增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;     // 推进当前位置
    return *this;
}