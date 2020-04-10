#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

template<class T>
class deque {
private:
    static const int bl=200;
    struct node{
        node *pre,*nxt;
        T *data;char mem[(2*bl+3)*sizeof(T)];
        int size;
        node(){pre=nxt=nullptr;size=0;data=(T*)mem;}
    };
    node *head=nullptr,*tail=nullptr;size_t sz;
public:
    class const_iterator;
    class iterator {
//        friend iterator insert(iterator pos, const T &value);
//        friend iterator erase(iterator pos);
        friend class deque;
        friend class const_iterator;
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        deque *a;
        node *x;int p;
    public:
        iterator():a(nullptr),x(nullptr),p(0){}
        iterator(deque *aa,node *xx,int pp):a(aa),x(xx),p(pp){}
        /**
         * return a new iterator which pointer n-next elements
         *   even if there are not enough elements, the behaviour is **undefined**.
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            if (n<0) return *this-(-n);
            //TODO
            node *t=x;int tp=p;int tot=n;
            while (t!=nullptr){
                if (tot<t->size-tp) break;
                tot-=t->size-tp;tp=0;
                t=t->nxt;
            }
            tp+=tot;
            if (t==nullptr && tp==0) return a->end();
            return iterator(a,t,tp);
        }
        iterator operator-(const int &n) const {
            if (n<0) return *this+(-n);
            //TODO
            node *t=x;int tp=p;int tot=n;
            while (t!=nullptr){
                if (tot<=tp) break;
                tot-=tp+1;
                t=t->pre;if (t) tp=t->size-1;
            }
            tp-=tot;
            return iterator(a,t,tp);
        }
        // return th distance between two iterator,
        // if these two iterators points to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            //TODO
            if (a!=rhs.a) throw invalid_iterator();
            node *t=x;int tp=p;int tot=0;
            while (t!=nullptr && t!=rhs.x){
                tot+=tp+1;
                t=t->pre;if (t) tp=t->size-1;
            }
            tot+=tp-rhs.p;
            if (t==nullptr){
                node *t=rhs.x;int tp=rhs.p;tot=0;
                while (t!=nullptr && t!=x){
                    tot-=tp+1;
                    t=t->pre;if (t) tp=t->size-1;
                }
                tot-=tp-p;
            }
            return tot;
        }
        iterator& operator+=(const int &n) {
            //TODO
            *this=*this+n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            //TODO
            *this=*this-n;
            return *this;
        }
        /**
         * TODO iter++
         */
        iterator operator++(int) {
            iterator tmp=*this;
            ++*this;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        iterator& operator++() {
            *this+=1;
            return *this;
        }
        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator tmp=*this;
            --*this;
            return tmp;
        }
        /**
         * TODO --iter
         */
        iterator& operator--() {
            *this-=1;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const {
            if (a==nullptr || x==nullptr || p>=x->size) throw invalid_iterator();
            return x->data[p];
        }
        /**
         * TODO it->field
         */
        T* operator->() const noexcept {
            if (a==nullptr || x==nullptr || p>=x->size) throw invalid_iterator();
            return x->data+p;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {return (x==rhs.x && p==rhs.p);}
        bool operator==(const const_iterator &rhs) const {return (x==rhs.it.x && p==rhs.it.p);}
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {return !(x==rhs.x && p==rhs.p);}
        bool operator!=(const const_iterator &rhs) const {return !(x==rhs.it.x && p==rhs.it.p);}
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
        friend class deque;
        friend class iterator;
    private:
        // data members.
        iterator it;
    public:
        const_iterator(){}
        const_iterator(const deque *aa,node *xx,int pp){it.a=const_cast<deque *>(aa);it.x=xx;it.p=pp;}
        const_iterator(const const_iterator &other):it(other.it){}
        const_iterator(const iterator &other):it(other){}
        const_iterator operator+(const int &n) const {return const_iterator(it+n);}
        const_iterator operator-(const int &n) const {return const_iterator(it-n);}
        int operator-(const const_iterator &rhs) const {return it-rhs.it;}
        const_iterator& operator+=(const int &n) {*this=*this+n;return *this;}
        const_iterator& operator-=(const int &n) {*this=*this-n;return *this;}
        const_iterator operator++(int) {return const_iterator(it++);}
        const_iterator& operator++() {++it;return *this;}
        const_iterator operator--(int) {return const_iterator(it--);}
        const_iterator& operator--() {--it;return *this;}
        T& operator*() const {return *it;}
        T* operator->() const noexcept {return &(*it);}
        bool operator==(const iterator &rhs) const {return it==rhs;}
        bool operator==(const const_iterator &rhs) const {return it==rhs.it;}
        bool operator!=(const iterator &rhs) const {return it!=rhs;}
        bool operator!=(const const_iterator &rhs) const {return it!=rhs.it;}
    };
    /**
     * TODO Constructors
     */
    deque() {head=tail=new node();sz=0;}
    deque(const deque &other) {
        head=tail=new node();sz=0;
        for (node *i=other.head;i!=nullptr;i=i->nxt){
            for (int j=0;j<i->size;j++) push_back(i->data[j]);
        }
    }
    /**
     * TODO Deconstructor
     */
    ~deque() {
        clear();
        delete head;
    }
    /**
     * TODO assignment operator
     */
    deque &operator=(const deque &other) {
        if (this==&other) return *this;
        clear();
        for (node *i=other.head;i!=nullptr;i=i->nxt){
            for (int j=0;j<i->size;j++) push_back(i->data[j]);
        }
        return *this;
    }
    /**
     * access specified element with bounds checking
     * throw index_out_of_bound if out of bound.
     */
    T & at(const size_t &pos) {
        if (pos<0 || pos>=sz) throw index_out_of_bound();
        node *t=head;int tot=pos;
        while (t!=nullptr){
            if (tot<t->size) break;
            tot-=t->size;
            t=t->nxt;
        }
        return t->data[tot];
    }
    const T & at(const size_t &pos) const {
        if (pos<0 || pos>=sz) throw index_out_of_bound();
        node *t=head;int tot=pos;
        while (t!=nullptr){
            if (tot<t->size) break;
            tot-=t->size;
            t=t->nxt;
        }
        return t->data[tot];
    }
    T & operator[](const size_t &pos) {return at(pos);}
    const T & operator[](const size_t &pos) const {return at(pos);}
    /**
     * access the first element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (sz==0) throw container_is_empty();
        return at(0);
    }
    /**
     * access the last element
     * throw container_is_empty when the container is empty.
     */
    const T & back() const {
        if (sz==0) throw container_is_empty();
        return at(sz-1);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {return iterator(this,head,0);}
    const_iterator cbegin() const {return const_iterator(this,head,0);}
    /**
     * returns an iterator to the end.
     */
    iterator end() {return iterator(this,tail,tail->size);}
    const_iterator cend() const {return const_iterator(this,tail,tail->size);}
    /**
     * checks whether the container is empty.
     */
    bool empty() const {return sz==0;}
    /**
     * returns the number of elements
     */
    size_t size() const {return sz;}
    /**
     * clears the contents
     */
    void clear() {
        node *t=head;
        while (t!=nullptr){
            node *tt=t->nxt;
            for (int i=0;i<t->size;i++) (&t->data[i])->~T();delete t;
            t=tt;
        }
        head=tail=new node();sz=0;
    }
    /**
     * inserts elements at the specified locat on in the container.
     * inserts value before pos
     * returns an iterator pointing to the inserted value
     *     throw if the iterator is invalid or it point to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {
        if (pos.a!=this || pos.x==nullptr) throw invalid_iterator();
        sz++;
        node *x=pos.x;
        for (int i=x->size;i>pos.p;i--) {
            new(&x->data[i])T(x->data[i-1]);(&x->data[i-1])->~T();
        }
        new(&x->data[pos.p])T(value);
        x->size++;
        if (x->size>=2*bl){
            node *t=new node();
            for (int i=bl;i<x->size;i++) {
                new(&t->data[i-bl])T(x->data[i]);(&x->data[i])->~T();
            }
            t->size=x->size-bl;x->size=bl;
            t->pre=x;t->nxt=x->nxt;x->nxt=t;
            if (t->nxt) t->nxt->pre=t;
            if (t->nxt==nullptr) tail=t;
            if (pos.p>=bl){
                pos.p-=bl;
                pos.x=t;
            }
        }
        return pos;
    }
    /**
     * removes specified element at pos.
     * removes the element at pos.
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid or it points to a wrong place.
     */
    iterator erase(iterator pos) {
        if (pos.a!=this || pos.x==nullptr) throw invalid_iterator();
        if (sz==0) throw container_is_empty();
        sz--;
        node *x=pos.x;(&x->data[pos.p])->~T();
        for (int i=pos.p;i<x->size-1;i++) {
            new(&x->data[i])T(x->data[i+1]);(&x->data[i+1])->~T();
        }
        x->size--;
        if (x->nxt && x->size+x->nxt->size<=bl){
            node *t=x->nxt;
            for (int i=0;i<t->size;i++) {
                new(&x->data[x->size+i])T(t->data[i]);(&t->data[i])->~T();
            }
            x->size+=t->size;x->nxt=t->nxt;
            if (x->nxt) x->nxt->pre=x;
            if (x->nxt==nullptr) tail=x;
            delete t;
        }
        if (x==head && x->size==0 && x!=tail){head=x->nxt;head->pre=nullptr;delete x;return begin();}
        if (x==tail && x->size==0 && x!=head) {tail=x->pre;tail->nxt=nullptr;delete x;return end();}
        if (x==tail && pos.p==x->size) return end();
        if (pos.p==x->size){x=x->nxt;pos.x=x;pos.p=0;}
        return pos;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {insert(end(),value);}
    /**
     * removes the last element
     *     throw when the container is empty.
     */
    void pop_back() {erase(end()-1);}
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {insert(begin(),value);}
    /**
     * removes the first element.
     *     throw when the container is empty.
     */
    void pop_front() {erase(begin());}
};

}

#endif

