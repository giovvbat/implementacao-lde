#include <iostream>
#include <sstream>
#include <string>
using namespace std;
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

namespace sc {
    template<typename T> 
    class list;
    template<typename T>
    class MyFowardIterator;
}

template<typename T>
class sc::MyFowardIterator {
    private:
    Node<T> *m_ptr;

    public:
    //aliases
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    //construtores
    MyFowardIterator(Node<T> *ptr=nullptr) { m_ptr=ptr; }
    MyFowardIterator(const MyFowardIterator &other) { m_ptr=other.m_ptr; }

    //sobrecarga de operadores
    MyFowardIterator& operator=(const MyFowardIterator &other) { m_ptr=other.m_ptr; return *this; }
    T& operator*() const { if(m_ptr != nullptr) return m_ptr->data; }
    MyFowardIterator operator++() { m_ptr=m_ptr->next; return MyFowardIterator{m_ptr}; }
    MyFowardIterator operator++(int) { MyFowardIterator dummy{*this}; m_ptr=m_ptr->next; return MyFowardIterator{dummy}; }
    MyFowardIterator operator--() { m_ptr=m_ptr->prev; return MyFowardIterator{m_ptr}; }
    MyFowardIterator operator--(int) { MyFowardIterator dummy{*this}; m_ptr=m_ptr->prev; return MyFowardIterator{dummy}; }
    bool operator==(const MyFowardIterator &rhs) const { return m_ptr == rhs.m_ptr; }
    bool operator!=(const MyFowardIterator &rhs) const { return m_ptr != rhs.m_ptr; }

};

template<typename T> 
class sc::list {

    //atributos
    private:
    Node<T> *m_head;
    Node<T> *m_tail;
    size_t m_len;

    public:
    //construtores
    //lista vazia
    list() { 
        m_head=new Node<T>(T(), nullptr, nullptr); 
        m_tail=new Node<T>(T(), nullptr, m_head); 
        m_head->next=m_tail; 
        m_len=0; 
    }

    //valores padrão
    explicit list(size_type count);

    //intervalo de ponteiros
    template<typename InputItr>
    list(InputItr first, InputItr last);

    //cópia de outra lista
    list(const list& other);

    //cópia de uma std::initializer_list
    list(initializer_list<T> ilist);

    //destrutor
    ~list();

    //sobrecarga de operador de atribuição para uma outra lista
    list& operator=(const list& other);

    //sobrecarga de operador de atribuição para uma std::initializer_list
    list& operator=(const initializer_list<T> ilist);

    //métodos de acesso
    //tamanho da lista
    size_type size() const { return m_len; }

    //retorna verdadeiro se a lista tiver vazia
    bool empty() const { return m_head->next==m_tail; }

    //retorna o primeiro valor da lista
    T& front() { 
        if(!empty()) 
            return m_head->next->data; 
        return T();
    }

    //retorna o último valor da lista
    T& back() { 
        if(!empty()) 
            return m_tail->prev->data;
        return T();
    }

    //retorna o último valor da lista de forma constante
    const T& front() const { 
        if(!empty()) 
            return m_head->next->data; 
        return T();
    }

    //retorna o último valor da lista de forma constante
    const T& back() const { 
        if(!empty()) 
            return m_tail->prev->data;
        return T();
    }

    //métodos de modificação
    //apaga o conteúdo da lista
    void clear();
    
    //adiciona um elemento no início da lista
    void push_front(const T& value);

    //adiciona um elemento no final da lista
    void push_back(const T& value);

    //remove o último elemento da lista
    void pop_back();

    //remove o primeiro elemento da lista
    void pop_front();

    //substitui o conteúdo da lista pelos elementos na range [first, last)
    template <class InItr> 
    void assign(InItr first, InItr last);

    //substitui o conteúdo da lista pelos elementos da ilist
    void assign(initializer_list<T> ilist) { assign(ilist.begin(), ilist.end()); }

    //método auxiliar para o merge sort
    void merge(list &other);

    //mergesort
    void sort();

    //friends
    //sobrecarga do ==
    friend bool operator==(const list& lhs, const list& rhs) {
        if(lhs.size() != rhs.size())
            return false;
        Node<T> *runl=lhs.m_head;
        Node<T> *runr=rhs.m_head;
        while(runl != lhs.m_tail) {
            if(runl->data != runr->data)
                return false;
            runl=runl->next;
            runr=runr->next;
        }
        return true;
    }

    //sobrecarga do !=
    friend bool operator!=(const list& lhs, const list& rhs) { return !(lhs == rhs); }

    //imprimir os elementos em uma string
    string to_string() {
        ostringstream oss;
        Node<T> *runner=m_head->next;
        while(runner != m_tail) {
            oss<<"["<<runner->data<<"]->";
            runner=runner->next;
        }
        oss<<"<null>";
        return oss.str();
    }

    //métodos de acesso com iteradores
    //retorna o primeiro nó da lista
    MyFowardIterator<T> begin() { return MyFowardIterator<T>(m_head->next); }

    //retorna o último nó da lista
    MyFowardIterator<T> end() { return MyFowardIterator<T>(m_tail); }

    //retorna o primeiro nó da lista de forma constante
    const MyFowardIterator<T> cbegin() const { return MyFowardIterator<T>(m_head->next); }

    //retorna o último nó da lista de forma constante
    const MyFowardIterator<T> cend() const { return MyFowardIterator<T>(m_tail); }

}; 

template<typename T>
sc::list<T>::list(size_type count) {
    m_head=new Node<T>(T(), m_tail, nullptr);
    m_tail=new Node<T>(T(), nullptr, m_head);
    Node<T> *p=m_head;
    for(auto i=0; i<count; i++) {
        Node<T> *nn=new Node<T>(T(), m_tail, p);
        p->next=nn;
        m_tail->prev=nn;
        p=nn;
    }
    m_len=count;
}

template<typename T>
template<typename InputItr>
sc::list<T>::list(InputItr first, InputItr last) {
    m_head=new Node<T>(T(), m_tail, nullptr);
    m_tail=new Node<T>(T(), nullptr, m_head);
    Node<T> *p=m_head;
    m_len=0;
    while(first != last) {
        Node<T> *nn=new Node<T>(*first, m_tail, p);
        p->next=nn;
        m_tail->prev=nn;
        p=nn;
        first++;
        m_len++;
    }
}

template<typename T>
sc::list<T>::list(const list& other) {
    m_head=new Node<T>(T(), m_tail, nullptr);
    m_tail=new Node<T>(T(), nullptr, m_head);
    Node<T> *runo=other.m_head->next;
    Node<T> *p=m_head;
    while (runo != other.m_tail) {
        Node<T> *nn=new Node<T>(runo->data, m_tail, p);
        p->next=nn;
        m_tail->prev=nn;
        p=nn;
        runo=runo->next;
    }
    m_len=other.m_len;
}

template<typename T>
sc::list<T>::list(initializer_list<T> ilist) {
    m_head=new Node<T>(T(), m_tail, nullptr);
    m_tail=new Node<T>(T(), nullptr, m_head);
    Node<T> *p=m_head;
    auto runner=ilist.begin();
    m_len=0;
    while(runner != ilist.end()) {
        Node<T> *nn=new Node<T>(*runner, m_tail, p);
        p->next=nn;
        m_tail->prev=nn;
        p=nn;
        runner++;
        m_len++;
    }
}

template<typename T>
sc::list<T>::~list() {
    Node<T> *current=m_head;
    while(current != nullptr) {
        Node<T> *next=current->next;
        delete current;
        current=next;
    }
}

template<typename T>
sc::list<T>& sc::list<T>::operator=(const list<T>& other) {
    if(this != &other) {
        Node<T> *current=m_head;
        while(current != nullptr) {
            Node<T> *next=current->next;
            delete current;
            current=next;
        }
        m_head=new Node<T>(T(), m_tail, nullptr);
        m_tail=new Node<T>(T(), nullptr, m_head);
        Node<T> *runo=other.m_head->next;
        Node<T> *p=m_head;
        while (runo != other.m_tail) {
            Node<T> *nn=new Node<T>(runo->data, m_tail, p);
            p->next=nn;
            m_tail->prev=nn;
            p=nn;
            runo=runo->next;
        }
    }
    m_len=other.m_len;
    return *this;
}

template<typename T>
sc::list<T>& sc::list<T>::operator=(const initializer_list<T> ilist) {
    Node<T> *current=m_head;
    while(current != nullptr) {
        Node<T> *next=current->next;
        delete current;
        current=next;
    }    
    m_head=new Node<T>(T(), m_tail, nullptr);
    m_tail=new Node<T>(T(), nullptr, m_head);
    auto runner=ilist.begin();
    Node<T> *p=m_head;
    m_len=0;
    while (runner != ilist.end()) {
        Node<T> *nn=new Node<T>(*runner, m_tail, p);
        p->next=nn;
        m_tail->prev=nn;
        p=nn;
        runner++;
        m_len++;
    }
    return *this;
}

template<typename T>
void sc::list<T>::clear() {
    Node<T> *current=m_head->next;
    while(current != m_tail) {
        Node<T> *next=current->next;
        delete current;
        current=next;
    }
    m_head->next=m_tail;
    m_tail->prev=m_head;
    m_len=0;
}

template<typename T>
void sc::list<T>::push_front(const T& value) {
    Node<T> *nn=new Node<T>(value, nullptr, nullptr);
    nn->next=m_head->next;
    m_head->next->prev=nn;
    nn->prev=m_head;
    m_head->next=nn;
    m_len++;
}

template<typename T>
void sc::list<T>::push_back(const T& value) {
    Node<T> *nn=new Node<T>(value, nullptr, nullptr);
    nn->next=m_tail;
    m_tail->prev->next=nn;
    nn->prev=m_tail->prev;
    m_tail->prev=nn;
    m_len++;
}

template<typename T>
void sc::list<T>::pop_back() {
    if(empty())
        return;
    Node<T> *target=m_head;
    while(target->next != m_tail)
        target=target->next;
    target->prev->next=m_tail;
    m_tail->prev=target->prev;
    delete target;
    m_len--;
}

template<typename T>
void sc::list<T>::pop_front() {
    if(empty())
        return;
    Node<T> *target=m_head->next;
    m_head->next=target->next;
    target->next->prev=m_head;
    delete target;
    m_len--;
}

template<typename T>
template <class InItr> 
void sc::list<T>::assign(InItr first, InItr last) {
    clear();
    m_len=0;
    while(first != last) {
        push_back(*first);
        first++;
    }
}

template<typename T>
void sc::list<T>::merge(list &other) {
    if(this == &other)
        return;
    Node<T> *runner=m_head->next;
    Node<T> *runnero=other.m_head->next;
    while(runner != m_tail && runnero != other.m_tail) {
        if(runner->data <= runnero->data)
            runner=runner->next;
        else {
            Node<T> *next=runnero->next;
            runner->prev->next=runnero;
            runnero->prev=runner->prev;
            runnero->next=runner;
            runner->prev=runnero;
            runnero=next;
        }
    }
    while(runnero != other.m_tail) {
        Node<T> *before=m_tail->prev;
        Node<T> *next=runnero->next;
        next=runnero->next;
        before->next=runnero;
        runnero->prev=before;
        runnero->next=m_tail;
        m_tail->prev=runnero;
        before=runnero;
        runnero=next;
    }
    m_len+=other.m_len;
    other.m_len=0;
    other.m_head->next=other.m_tail;
    other.m_tail->prev=other.m_head;
}

template<typename T>
void sc::list<T>::sort() {
    if(m_len<2)
        return;
    size_type len=m_len/2;
    MyFowardIterator<T> lb=begin(), re=end(), le=lb;
    for(auto i=0;i<len;i++)
        ++le;
    sc::list<T> L(lb, le);
    sc::list<T> R(le, re);
    //cout<<L.to_string()<<endl;
    L.sort();
    R.sort();
    R.merge(L);
}

int main() {
    //int A[]={9, 8, 7, 6, 5, 6, 4, 0};
    //auto n=sizeof(A)/sizeof(int);
    initializer_list<int> numbers={3, 2, 1};
    sc::list<int> lista(numbers);
    sc::list<int> list(lista.begin(), lista.end());

    list.sort();

    cout<<list.to_string();
    
    return 0;
}