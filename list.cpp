#include <iostream>
#include <sstream>
#include <string>
using namespace std;

namespace sc {
    using size_type=unsigned long;

    template<typename T> 
    struct Node {
        T data;
        Node<T> *next;
        Node<T> *prev;

        Node(const T &d=T(), Node<T> *n=nullptr, Node<T> *p=nullptr) {
            data=d;
            next=n;
            prev=p;
        }
    };

    template<typename T> 
    class list {
    //atributos
    private:
    Node<T> *head;

    public:
    //aliases
    using value_type=T;
    using pointer=T*;
    using reference=T&;
    using const_reference=const T&;

    //construtores
    //lista vazia
    list() { head=new Node<T>(T(), nullptr, nullptr); }
    //valores padrão
    explicit list(size_type count) {
        head=new Node<T>(T(), nullptr, nullptr);
        Node<T> *p=head;
        for(auto i=0; i<count; i++) {
            Node<T> *nn=new Node<T>(i+1, nullptr, p);
            p->next=nn;
            p=nn;
        }
    }
    //intervalo de ponteiros
    template<typename InputItr>
    list(InputItr first, InputItr last) {
        head=new Node<T>(T(), nullptr, nullptr);
        Node<T> *p=head;
        while(first != last) {
            Node<T> *nn=new Node<T>(*first, nullptr, p);
            p->next=nn;
            p=nn;
            first++;
        }
    }
    //cópia de outra lista
    list(const list& other) {
        head=new Node<T>(T(), nullptr, nullptr);
        Node<T> *runo=other.head->next;
        Node<T> *p=head;
        while (runo != nullptr) {
            Node<T> *nn=new Node<T>(runo->data, nullptr, p);
            p->next=nn;
            p=nn;
            runo=runo->next;
        }
    }
    //cópia de uma std::initializer_list
    list(initializer_list<T> ilist) {
        head=new Node<T>(T(), nullptr, nullptr);
        Node<T> *p=head;
        auto runner=ilist.begin();
        while(runner != ilist.end()) {
            Node<T> *nn=new Node<T>(*runner, nullptr, p);
            p->next=nn;
            p=nn;
            runner++;
        }
    }
    //destrutor
    ~list() {
        Node<T> *current=head;
        while(current != nullptr) {
            Node<T> *next=current->next;
            delete current;
            current=next;
        }
    }
    //sobrecarga de operador de atribuição para uma outra lista
    list& operator=(const list& other) {
        if(this != &other) {
            Node<T> *current=head;
            while(current != nullptr) {
                Node<T> *next=current->next;
                delete current;
                current=next;
            }
            head=new Node<T>(T(), nullptr, nullptr);
            Node<T> *runo=other.head->next;
            Node<T> *p=head;
            while (runo != nullptr) {
                Node<T> *nn=new Node<T>(runo->data, nullptr, p);
                p->next=nn;
                p=nn;
                runo=runo->next;
            }
        }
        return *this;
    }
    //sobrecarga de operador de atribuição para uma std::initializer_list
    list& operator=(initializer_list<T> ilist) {
        Node<T> *current=head;
        while(current != nullptr) {
            Node<T> *next=current->next;
            delete current;
            current=next;
        }    
        head=new Node<T>(T(), nullptr, nullptr);
        auto runner=ilist.begin();
        Node<T> *p=head;
        while (runner != ilist.end()) {
            Node<T> *nn=new Node<T>(*runner, nullptr, p);
            p->next=nn;
            p=nn;
            runner++;
        }
        return *this;
    }

    //métodos de acesso
    //tamanho da lista
    size_type size() const {
        Node<T> *runner=head->next;
        size_type count=0;
        while(runner != nullptr) {
            count++;
            runner=runner->next;
        }
        return count;
    }
    //retorna verdadeiro se a lista tiver vazia
    bool empty() const { return head->next==nullptr; }
    //retorna o primeiro valor da lista
    const_reference front() const { if(!empty()) return head->next; }
    //imprimir os elementos em uma string
    string to_string() {
        ostringstream oss;
        Node<T> *runner=head->next;
        while(runner != nullptr) {
            oss<<"["<<runner->data<<"]->";
            runner=runner->next;
        }
        oss<<"<null>";
        return oss.str();
    }
    };
};

int main() {
    //int A[]={9, 8, 7, 6, 5, 6, 4, 0};
    //auto n=sizeof(A)/sizeof(int);
    sc::list<int> lista;
    initializer_list<int> numbers={6, 6, 6, 6, 7, 8, 99, 0, 6};
    sc::list<int> list(numbers);

    cout<<lista.size()<<endl;
    if(lista.empty())
        cout<<"tá vazio"<<endl;
    lista=list;
    cout<<lista.size();
    if(lista.empty())
        cout<<"tá vazio"<<endl;

}