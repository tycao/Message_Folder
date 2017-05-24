#include <iostream>
#include <set>
#include <string>
using namespace std;

class Folder;
class Message
{
    friend class Folder;
public:
    explicit Message(const string &str = "") : contents(str){ }
    Message(const Message &m);
    Message& operator=(const Message&);
    ~Message();
    void addFldr(Folder*);
    void remFldr(Folder*);

    //add or delete this message to or from a Folder
    void save(Folder&);
    void remove(Folder&);
private:
    string contents;
    std::set<Folder*> folders;

    void add_to_Folders(const Message&);
    void remove_from_Folders();
};

void Message::addFldr(Folder* f)
{
    this->folders.insert(f);
}
Message::Message(const Message& m) : contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);
}

Message::~Message()
{
    remove_from_Folders();
}
Message& Message::operator=(const Message& m)
{
    remove_from_Folders();
    contents = m.contents;
    folders = m.folders;
    add_to_Folders(m);
    return *this;
}

class Folder
{
public:
    void addMsg(Message* m){ this->msgs.insert(m); }
    void remMsg(Message* m){ this->msgs.erase(m); }
    void add_to_Messages(const Folder&);
    void remove_from_Msgs();
    Folder() = default;
    Folder(const Folder& f) : msgs(f.msgs){ add_to_Messages(f); }
    ~Folder() { remove_from_Msgs(); }
    Folder& operator=(const Folder&);
    void showMsg() { for (const Message* msg : msgs) cout << msg->contents << endl; }
private:
    std::set<Message*> msgs;
};
Folder& Folder::operator=(const Folder& f)
{
    remove_from_Msgs();
    msgs = f.msgs;
    add_to_Messages(f);
    return *this;
}
void Folder::add_to_Messages(const Folder& f)
{
    for (auto msg : f.msgs)
        msg->addFldr(this);
}
void Folder::remove_from_Msgs()
{
    for (auto m : this->msgs)
        m->remFldr(this);
}
void Message::save(Folder& f)
{
    this->folders.insert(&f);
    f.addMsg(this);
}
void Message::remove_from_Folders()
{
    for (auto f : this->folders)
        f->remMsg(this);
}
void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.remMsg(this);
}
void Message::add_to_Folders(const Message& m)
{
    for (auto f : m.folders)
        f->addMsg(this);
}
void Message::remFldr(Folder* f)
{
    folders.erase(f);
    //f->remMsg(this);
}
int main()
{
    Message msg("Hello Email from tycao");
    Folder f;
    msg.save(f);
    f.showMsg();
    Folder f2(f);
    f2.showMsg();
    cout << "++++++++++++++++" << endl;
    Message msg2("email test from Jim Hick");
    msg2.save(f);
    f.showMsg();
    cout << "+++++++++++++++" << endl;
    msg = msg2;
    f.showMsg();
    return 0;
}
