/*
Student Name: Berkay Özerbay
Student Number: 2016400270
Project Number: 1
Operating System: Mac OS
Compile Status:     Compiled
Program Status: Succesful
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

#include "BigInteger.h"
#include <algorithm>
#include <stack>

/*ostream &operator<<(ostream &out, const BigInteger &bigInteger)  {
    string str = "";
    Node *head = bigInteger.num->head;
    while (head) {
        str += to_string(head->data);
        head = head->next;
    }
    reverse(str.begin(), str.end());
    if (str == "")
        str = "0";
    out << str;
    return out;
}*/
//Copy Constructor from an integer
BigInteger::BigInteger(int number) {
    this->num=new LinkedList;

    while(number>10){
        this->num->pushTail(number%10);
        number=number/10;
    }
    this->num->pushTail(number);
}



// Copy Constructor from a string
BigInteger::BigInteger(const string& bigInteger){
    this->num=new LinkedList();
    for(int i=0;i<bigInteger.size();i++){
        int j= bigInteger[i]-'0';
        this->num->pushHead(j);
    }


}
//Copy Constructor from a BigInteger
BigInteger::BigInteger(const BigInteger &other){
    Node* temp=other.num->head;
    this->num = new LinkedList();
    while(temp){
        this->num->pushTail(temp->data);
        temp=temp->next;
    }
}
//Addition operation of two BigIntegers
BigInteger BigInteger::operator+(const BigInteger &list){

    Node* ptr1=this->num->head;
    Node* ptr2=list.num->head;
    int c=0;
    int sum=0;
    BigInteger final;
    final.num=new LinkedList;
    while(ptr1 !=NULL &&ptr2!=NULL){
        sum=ptr1->data+ptr2->data+c;
        final.num->pushTail((sum%10));
        ptr1=ptr1->next;
        ptr2=ptr2->next;
        c=(sum/10);

    }


    if(ptr1!=NULL){
        while(ptr1!=NULL) {
            sum = ptr1->data + c;
            final.num->pushTail((sum%10));
            c = sum / 10;
            ptr1 = ptr1->next;
        }
    }
    else if(ptr2!=NULL){
        while(ptr2!=NULL) {
            sum = ptr2->data + c;
            final.num->pushTail((sum%10));
            c = sum / 10;
            ptr2 = ptr2->next;
        }
    }


    if(c!=0)
         final.num->pushTail(c);//If there was one more carry
    return final;
        }


//assignment operator for BigInteger
BigInteger& BigInteger::operator=(const BigInteger &list){
    Node *tmp=list.num->head;
    this->num->~LinkedList();
    this->num=new LinkedList;
    while(tmp!=NULL){
        this->num->pushTail(tmp->data);
        tmp=tmp->next;
    }
    return *this ;
}

//Multiplication of an integer and BigInteger

BigInteger BigInteger::operator*(int i) {
    int j;
    BigInteger final;
    final.num=new LinkedList;
    if(i==0) {
        final.num->pushTail(0);
    }
    else {
        for(j=0;j<i;j++){
            final=final+(*this);
        }
    }

    return final;

}
//Multiplication of two BigInteger
BigInteger BigInteger::operator*(const BigInteger &list) {
    BigInteger final;
    BigInteger temp;
    int c=0;
    Node* ptr=list.num->head;
    final.num=new LinkedList;

    while(ptr!=NULL){
        temp=(*this)*(ptr->data);//Multiplication of integer and BigInt
        ptr=ptr->next;
        for(int i=0;i<c;i++){
           temp.num->pushHead(0);
        }
        final=final+temp;//BigInt Addition
        c++;
    }
    return final;
}

//Destructor
BigInteger::~BigInteger(){
    this->num->~LinkedList();
}


