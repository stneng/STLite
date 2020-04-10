#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
    struct node{
        node(T da,int di=1):data(da){dist=di;l=r=nullptr;}
        T data;int dist;
        node *l,*r;
    };
    node *merge(node *a,node *b){
        if (a==nullptr) return b;if (b==nullptr) return a;
        if (Compare()(a->data,b->data)) std::swap(a,b);
        a->r=merge(a->r,b);
        if (a->l==nullptr || a->l->dist<a->r->dist) std::swap(a->l,a->r);
        if (a->r) a->dist=a->r->dist+1;else a->dist=1;
        return a;
    }
    node *copy(node *x){
        if (x==nullptr) return x;
        node *tmp=new node(x->data,x->dist);
        tmp->l=copy(x->l);tmp->r=copy(x->r);
        return tmp;
    }
    void del(node *x){
        if (x==nullptr) return;
        del(x->l);del(x->r);
        delete x;
    }
    node *root;size_t sz;
public:
	/**
	 * TODO constructors
	 */
    priority_queue() {root=nullptr;sz=0;}
    priority_queue(const priority_queue &other) {
        root=copy(other.root);sz=other.sz;
    }
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
        del(root);
    }
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
        if (this==&other) return *this;
        del(root);
        root=copy(other.root);sz=other.sz;
        return *this;
    }
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
        if (sz){
            return root->data;
        }else{
            throw container_is_empty();
        }
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
        node *tmp=new node(e);
        root=merge(root,tmp);
        sz++;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
        if (sz){
            node *tl=root->l,*tr=root->r;
            delete root;
            root=merge(tl,tr);
            sz--;
        }else{
            throw container_is_empty();
        }
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
        return sz;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
        return (sz==0);
	}
	/**
	 * return a merged priority_queue with at least O(logn) complexity.
	 */
	void merge(priority_queue &other) {
        root=merge(root,other.root);
        sz+=other.sz;
        other.root=nullptr;other.sz=0;
	}
};

}

#endif
