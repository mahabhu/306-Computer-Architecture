#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef double db;
#define test   ll t; cin>> t; for(ll tc=1; tc<=t; tc++)
#define caase  printf("Case %lld: ", tc);
#define arrays ll n; cin>> n; ll a[n]; for(ll i=0; i<n; i++) cin>> a[i];
#define showA  for(ll i=0; i<n; i++) cout<< a[i]<< " "; cout<< endl;
#define debug  cout<< "oh no much problem\n";
#define DOUBLE_MAX 1000000000.00

#define pll pair<ll,ll>
#define pld pair<ll,double>
#define pdl pair<double,ll>
#define pb  push_back
#define mp  make_pair
#define pi  acos(-1.0)
//#define x   first
//#define y   second
#define N   5000009
#define mod 1000000007
#define mod2 1621836843

string hexa[16] = {"0000","0001","0010","0011",
                  "0100","0101","0110","0111",
                  "1000","1001","1010","1011",
                  "1100","1101","1110","1111"};

string ins[16] = {"srl","sw","andi","addi",
                  "subi","lw","sll","or",
                  "ori","j","bneq","and",
                  "beq","add","sub","nor"};
ll linecnt = 0;
ll labelCnt = 0;

string newLabel(){
        string h = to_string(labelCnt++);
        h = "wowlabel"+h;
        return h;
    }

string dec2str(ll n, ll b){
    if(b==4){
        return hexa[n%16];
    }
    return hexa[n/16]+hexa[n%16];
}

ll ins2dec(string s){
    for(ll i=0; i<16; i++){
        if(ins[i]==s) return i;
    }
    return -1;
}

ll hex2dec(string s){
    for(ll i=0; i<16; i++){
        if(hexa[i]==s) return i;
    }
    return -1;
}

string str2hex(string s){
    string h = "";
    for(ll i=0; i<16; i+=4){
        string g = s.substr(i,4);
        ll k = hex2dec(g);
        char f;
        (k<=9)? f = '0'+k : f = 'a'+ (k-10);
        h+=f;
    }
    return h;
}

map<string,ll> M;
vector<vector<string>> fullasm;

vector<string>split(string s)
{
    vector<string> elements;
    string item = "";
    for(ll i=0; i<s.size(); i++) {
        if (s[i]==' ' || s[i]==',' || s[i]=='\t' || s[i]==':') {
            if (item != "") {
                elements.pb(item);
                item = "";
            }
        }
        else {
            item+=s[i];
        }
    }
    if(item!="") {
        elements.push_back(item);
    }
    return elements;
}

string convert(vector<string> line){
    vector<string> v = line;
    if(ins2dec(v[0])==-1){
        M[v[0]]=linecnt;
        //cout<< v[0]<< " "<< M[v[0]]<< endl;
        vector<string> f;
        for(ll i=1; i<v.size(); i++) f.pb(v[i]);
        v = f;
    }
    if(v.size()==0) return "";

    string insfin = "";
    insfin+=hexa[ins2dec(v[0])];

    //insfin+=" ";

    if(v[0]=="or" || v[0]=="and" || v[0]=="add" || v[0]=="sub" || v[0]=="nor" ){
        insfin+=hexa[M[v[2]]];
        insfin+=hexa[M[v[3]]];
        insfin+=hexa[M[v[1]]];
        return insfin;
    }
    if(v[0]=="srl" || v[0]=="sll"|| v[0]=="ori"|| v[0]=="andi"|| v[0]=="addi"|| v[0]=="subi"){
        insfin+=hexa[M[v[2]]];
        insfin+=hexa[M[v[1]]];
        insfin+=bitset<4>(stoi(v[3])).to_string();
        return insfin;
    }
    if(v[0]=="beq" || v[0]=="bneq"){
        insfin+=hexa[M[v[2]]];
        insfin+=hexa[M[v[1]]];
        insfin+=bitset<4>(M[v[3]]-linecnt-1).to_string();
        return insfin;
    }
    if(v[0]=="lw" || v[0]=="sw"){

        string f = v[2];
        stringstream ss1(f);
        string g1;
        getline(ss1,g1,'(');
        string g2;
        getline(ss1,g2,')');

        insfin+=hexa[M[g2]];
        insfin+=hexa[M[v[1]]];
        insfin+=bitset<4>(stoi(g1)).to_string();
        return insfin;
    }
    if(v[0]=="j"){
        insfin+=bitset<8>(M[v[1]]).to_string();
        insfin+="0000";
        return insfin;
    }
    return "";
}



int main()
{

    M["$zero"]=0;
    M["$t0"]=1;
    M["$t1"]=2;
    M["$t2"]=3;
    M["$t3"]=4;
    M["$t4"]=5;
    M["$sp"]=6;

    ifstream f0("assembly.txt");
    linecnt = 0;
    if(f0.is_open()){
        string line;
        while(getline(f0,line)){
            vector<string> vv = split(line);
            if(vv.size()>=2 && vv[vv.size()-2]=="push"){
                ll k = vv.size();
                vv[k-2]="sw";
                vv.pb("0($sp)");
                fullasm.pb(vv);
                fullasm.pb({"subi","$sp","$sp","1"});
            }
            else if(vv.size()>=2 && vv[vv.size()-2]=="pop"){
                ll k = vv.size();
                string h = vv[k-1];
                vv[k-2]="addi";
                vv[k-1]="$sp";
                vv.pb("$sp");
                vv.pb("1");

                fullasm.pb(vv);
                fullasm.pb({"lw",h,"0($sp)"});
            }
            else if(vv.size()>=4 && vv[vv.size()-4]=="beq"){
                ll k = vv.size();
                string h = vv[k-1];
                vv[k-4]="bneq";
                vv[k-1]=newLabel();
                M[vv[k-1]] = fullasm.size()+2;
                fullasm.pb(vv);
                fullasm.pb({"j",h});
            }
            else if(vv.size()>=4 && vv[vv.size()-4]=="bneq"){
                ll k = vv.size();
                string h = vv[k-1];
                vv[k-4]="beq";
                vv[k-1]=newLabel();
                M[vv[k-1]] = fullasm.size()+2;
                fullasm.pb(vv);
                fullasm.pb({"j",h});
            }
            else if(vv.size()>0){
                fullasm.pb(vv);
            }
        }
    }
    f0.close();
    for(ll i=0; i<fullasm.size(); i++){
        cout<< i<< " : ";
        for(ll j=0; j<fullasm[i].size(); j++){
            cout<< fullasm[i][j]<< " _ ";
        }
        cout<< endl;
    }
    linecnt = 0;
    for(ll i=0; i<fullasm.size(); i++){
        linecnt = i;
        string insfin = convert(fullasm[i]);
    }
    linecnt = 0;
    vector<string> instructions;

    linecnt = 0;
    for(ll i=0; i<fullasm.size(); i++){
        linecnt = i;
        string insfin = convert(fullasm[i]);
        if(insfin.size()==16) instructions.pb(str2hex(insfin));
    }

    cout<< "unsigned int instruction[256]={\n";
    for(ll i=0; i<256; i++){
        cout<< "0x";
        (i<instructions.size())? cout<< instructions[i] : cout<< "0000";
        if(i<255) cout<< ",";
        else cout<< "};";
        if(i%16==15) cout<< "\n";

    }
}


/*
addi $t1, $zero, 3
     subi $t2, $zero, -2
     add $t0, $t1, $t2
     sub $t3, $t1, $t2
     nor $t4, $t0, $t2
     sw $t1, 3($t2)
     srl $t2, $t2, 1
     beq $t2, $t3, label1
     j end
 label1:  sll $t3, $t3, 1
     lw $t2, 4($t2)
     j label2
 label3:    or $t0, $t0, $t2
     andi $t2, $t4, 1
     ori $t1, $t1, 5
     and $t1, $t2, $t4
     j end
 label2:    bneq $t0, $t2, label3
 end: addi $t0, $zero, -7
	push $t0
	pop $t1
	beq $t0, $t1, end




*/
