 #include <bits/stdc++.h>
 #define PLUS 0
 #define MINUS 1
 #define NONE 2
 #define TIMES 3
 #define SIN 4
 #define DIVIDE 5
 #define POWER 6
 using namespace std;
 class my{
     public:
     bool isNum;
     bool isNode;
     double num;
     double d;
     int type;// 0-plus, 1-minus, 2-none, 3-times, 4-sin, 5-divide, 6-power(only positive int allowed)
     int left,right;
     int self_node;
     my(double x=0, double dd=0, int t=0, int l=0,int r=0,int s=0,bool isnum=0,bool isnode=0)
     {
         num=x;
         d=dd;
         type=t;
         left=l;
         right=r;
         self_node=s;
         isNode=isnode;
         isNum=isnum;
         return;
     }
 };
 my x[100];
 double power(double x,double y)
 {
     int temp=y;
     if (temp==1) return x;
     if (temp==2) return x*x;
     if (temp==0) return 1;
     temp=max(1,temp);
    cout<<y<<endl;
     if (temp%2==0) return power(x,temp/2)*power(x,temp/2);
     else return x*power(x,temp/2)*power(x,temp/2);
 }
 double getNum(my x1,my x2,int t)
 {
     if (t==0) return x1.num+x2.num;
     if (t==1) return x1.num-x2.num;
     if (t==3) return x1.num*x2.num;
     if (t==4) return sin(x1.num);
     if (t==5){
     try{
      return x1.num/x2.num;}
     catch(...)
     {
         cout<<"Wrong : Divided by 0."<<endl;
         return 0;
     }}
     if (t==6) return power(x1.num,x2.num);
     cout<<"wrong"<<endl;
     return 0;
 }
 double getD(my x1,my x2,int t)
 {
     if (t==0) return x1.d+x2.d;
     if (t==1) return x1.d-x2.d;
     if (t==3) return x1.d*x2.num+x1.num*x2.d;
     if (t==4) return cos(x1.num)*x1.d;
     if (t==5) { try{return (x1.d*x2.num-x1.num*x2.d)/((x2.num)*(x2.num));}catch(...){         cout<<"Wrong : Divided by 0."<<endl;
         return 0;
}}
     if (t==6) return (x2.num-1)*(power(x1.num,x2.num-1))*(x1.d);
     cout<<"wrong"<<endl;

     return 0;
 }
void dd(int to)
 {
     int l,r;
     if (x[to].isNode) return;
     //cout<<"get"<<to<<endl;
     l=x[to].left;
     r=x[to].right;
     if (x[l].isNum ==false) dd(l);
     if (x[r].isNum ==false) dd(r);
     x[to].num=getNum(x[l],x[r],x[to].type);
     //cout<<x[to].num<<endl;
     x[to].isNum=true;
     x[to].d=getD(x[l],x[r],x[to].type);
     return;
 }
 int main()
 {
     // f(x,y)=(((x+y)-(x-y)) * (x-y) / (sin(x+y)))^(y-x) 
     // dx/x=1,dy/x=2;
     // x=2,y=3;
     x[0]=my(2,1,NONE,0,0,0,true,true);
     x[1]=my(3,2,NONE,0,0,1,true,true);
     x[2]=my(0,0,PLUS,0,1,2,false,false);
     x[3]=my(0,0,MINUS,0,1,3,false,false);
     x[4]=my(0,0,MINUS,2,3,4,false,false);
     x[5]=my(0,0,TIMES,4,3,5,false,false);
     x[6]=my(0,0,SIN,2,0,6,false,false);
     x[7]=my(0,0,DIVIDE,5,6,7,false,false);
     x[8]=my(0,0,MINUS,1,0,8,false,false);
     x[9]=my(0,0,POWER,7,8,9,false,false);
     dd (9);
    cout<<x[0].num<<" "<<x[0].d<<endl;
    cout<<x[1].num<<" "<<x[1].d<<endl;
    cout<<x[2].num<<" "<<x[2].d<<endl;
    cout<<x[3].num<<" "<<x[3].d<<endl;
    cout<<x[4].num<<" "<<x[4].d<<endl;
    cout<<x[5].num<<" "<<x[5].d<<endl;
    cout<<x[6].num<<" "<<x[6].d<<endl;
    cout<<x[7].num<<" "<<x[7].d<<endl;
    cout<<x[8].num<<" "<<x[8].d<<endl;
    cout<<x[9].num<<" "<<x[9].d<<endl;

     return 0;
 }
