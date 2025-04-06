#include<iostream>
#include<string>
using namespace std;

void LH_s(string b,string&a){
    a=b;
for(int i=0;i<a.length();i+=4)    
    {
    string str=a.substr(i,4);
    int t=(str[0]-48)*8+(str[1]-48)*4+(str[2]-48)*2+(str[3]-48);
        switch(t){
            case 0:a.replace(i,4,"1110");break;
            case 1:a.replace(i,4,"0100");break;
            case 2:a.replace(i,4,"1101");break;
            case 3:a.replace(i,4,"0001");break;
            case 4:a.replace(i,4,"0010");break;
            case 5:a.replace(i,4,"1111");break;
            case 6:a.replace(i,4,"1011");break;
            case 7:a.replace(i,4,"1000");break;
            case 8:a.replace(i,4,"0011");break;
            case 9:a.replace(i,4,"1010");break;
            case 10:a.replace(i,4,"0110");break;
            case 11:a.replace(i,4,"1100");break;
            case 12:a.replace(i,4,"0101");break;
            case 13:a.replace(i,4,"1001");break;
            case 14:a.replace(i,4,"0000");break;
            case 15:a.replace(i,4,"0111");break;
        }
    }
}


void LH_p(string c,string &a){
    a=c;
    a[1]=c[4];
    a[2]=c[8];
    a[3]=c[12];
    a[4]=c[1];
    a[6]=c[9];
    a[7]=c[13];
    a[8]=c[2];
    a[9]=c[6];
    a[11]=c[14];
    a[12]=c[3];
    a[13]=c[7];
    a[14]=c[11];
}

void yh(string a,string b,string&c){
    c.resize(a.length());
    for(int i=0;i<a.length();i++)
    {   if(a[i]!=b[i])
        c[i]='1';
        else c[i]='0';
    }
}//异或操作
int main(){
const int l=4,m=4,Nr=4;
string k[5],u[4],w[4],v[4];
string x,K,y;
cin>>x;
cin>>K;
for(int i=1;i<=5;i++){
    k[i-1]=K.substr(4*i-4,16);
}
w[0]=x;
for(int i=0;i<Nr;i++){
yh(w[i],k[i],u[i]);
LH_s(u[i],v[i]);
if(i!=Nr-1)LH_p(v[i],w[i+1]);
}
yh(v[3],k[4],y);
cout<<y;
    return 0;
}