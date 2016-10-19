#include<bits/stdc++.h>
using namespace std;
#define debug cout<<"debug"<<"\n";
int PC_1[8][7],LS[17],PC_2[8][6],C[17][28],D[17][28],IP[8][8],E[8][6];
int L[17][32],R[17][32],Plain[64],F[32];
int S[9][4][16],P[8][4],IP_1[8][8];
int Key[56],K[17][48];
string n;
void initTable(){
	ifstream myfile("file.in"); // semua data tabel ada pada file.in	
	for(int i=0;i<8;i++){	
		for(int j=0;j<7;j++){
			myfile >> PC_1[i][j];
		}
	}
	for(int i=1;i<=16;i++){
		myfile >> LS[i];
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<6;j++){
			myfile >> PC_2[i][j];
		}
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			myfile >> IP[i][j];
		}
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<6;j++){
			myfile >> E[i][j];	
		}
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<4;j++){
			for(int k=0;k<16;k++){
				myfile >> S[i][j][k];
			}
		}
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<4;j++){
			myfile >> P[i][j];
		}
	}
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			myfile >> IP_1[i][j];
		}
	}
	myfile.close();
}
void inputKey(){
	string inp;
	cout << "Masukkan Key : \n";
	getline(cin,inp);
	int tKey[64];
	for(int i=0;i<inp.length();i++){
		tKey[i]=(int)inp[i]-(int)'0';
	}
	for(int i=0;i<56;i++) {
		Key[i]=tKey[PC_1[i/7][i%7]-1];
	}
}
void buildCD(){
	for(int i=0;i<28;i++){
		C[0][i]=Key[i];
		D[0][i]=Key[28+i];
	}
	for(int i=1;i<17;i++){
		int k=0;
		for(int j=LS[i];j<LS[i]+28;j++,k++){
			C[i][k]=C[i-1][j%28];
			D[i][k]=D[i-1][j%28];
		}
	}
}
void buildK(){
	int tKey[56];
	for(int i=1;i<=16;i++){
		for(int j=0;j<28;j++){
			tKey[j]=C[i][j];
			tKey[j+28]=D[i][j];
		}
		//for(int j=0;j<56;j++)cout<<tKey[j];cout<<"\n";
		//for(int j=0;j<56;j++) cout<<tKey[j];cout<<"\n";
		for(int j=0;j<48;j++){
			K[i][j]=tKey[PC_2[j/6][j%6]-1];
//			cout<<K[i][j]<<" ";
		}//cout<<"\n";
	}	
}
 
string charToBit(char c){
	int m = (int) c;
	string ret="";
	while(m!=0){
		ret = (char)((int)'0' + m%2) + ret;
		m/=2;
	}
	while(ret.length()!=8) ret = '0' + ret;
	return ret;
}
void buildLR(string inp){
	for(int i=0;i<64;i++){
		Plain[i]=(int)inp[IP[i/8][i%8]-1] - (int)'0';
	}
	for(int i=0;i<32;i++){
		L[0][i]=Plain[i];
		R[0][i]=Plain[i+32];
	}
}
void insertToF(int m,int val){
	int temp=val;
	for(int i=m*4+3;i>=m*4;i--){
		F[i]=temp%2;
		temp/=2;
	}
//	cout<<"Berhasil F pada "<<m<<"\n";
}
void buildF(int m){
	int Rexp[48],j,k; 
	for(int i=0;i<48;i++){
		Rexp[i]=R[m-1][E[i/6][i%6]-1]^K[m][i];
//		cout<<Rexp[i];
	}//cout<<"\n";
	for(int i=0;i<8;i++){
//		debug
		j=Rexp[i*6]*2+Rexp[(i+1)*6-1]*1;
		k=Rexp[i*6+1]*8 +Rexp[i*6+2]*4 +Rexp[i*6+3]*2 +Rexp[i*6+4]*1;
//		cout<<i<<" "<<j<<" "<<k<<"\n";
		insertToF(i,S[i][j][k]);
//		debug
	}
}
string intToHexa(int val){
	int m=val,x;string ret="";
	for(int i=1;i<=2;i++){
		x=m%16;
		if(x>9){
			ret= (char)((int)'A' + x-10) + ret;
		} else ret = (char)(x+(int)'0')+ret;
		m/=16;
	}
	return ret;
}
string bitToHexa(string inp){
	string temp="",ret="";
	for(int i=0;i<inp.length();i++){
		temp+=inp[i];
		if(temp.length()==8){
			int x=0,k=1;
			for(int j=7;j>=0;j--,k*=2){
				x+=((int)temp[j]-(int)'0')*k;
			} 
			//cout<<x<<"\n";
			ret+=intToHexa(x);
			temp="";
		}
	}	
	return ret;
}
string bitToString(string inp){
	string temp="",ret="";
	for(int i=0;i<inp.length();i++){
		temp+=inp[i];
		if(temp.length()==8){		
			int k=1,m=0;
			for(int j=7;j>=0;j--){
				m+= k*((int)temp[j]-'0');
				k*=2;
			}
			ret = ret + (char)m;
			temp="";
		}
	}
	return ret;
}
string enc(string inp){
	buildLR(inp);
	string ret="",temp;
	for(int i=0;i<64;i++)ret+='0';
	temp=ret;
	for(int i=1;i<=16;i++){
		for(int j=0;j<32;j++){
			L[i][j]=R[i-1][j];
		}
		buildF(i);
		for(int j=0;j<32;j++){
//			if(i==1)cout<<L[i-j][j]<<" "<<F[j]<<"\n";
			R[i][j]=L[i-1][j]^F[j];
		}
	}
	for(int i=0;i<32;i++){
		temp[i+32]=(char)(L[16][i]+(int)'0');
		temp[i]=(char)(R[16][i]+(int)'0');
	}
	for(int i=0;i<64;i++){
		ret[i]=temp[IP_1[i/8][i%8]-1];
	}
	if(n[0]=='1')return bitToHexa(ret); else return bitToString(ret);
}
string encrypt(string inp){
	string ret="",temp="",inp1="",inp2="";
	for(int i=0;i<inp.length();i++){
		inp1 = inp1 + charToBit(inp[i]);
	}
	for(int i=0;i<inp1.length();i++){
		inp2+=inp1[i];
		if(inp2.length()==64){
			ret+=enc(inp2);
			inp2="";
		}
	}
	//cout<<ret<<"\n";
	return ret;
}
string hexaToBit(string inp){
	int m=0,k=16;
	string ret="";
	for(int i=0;i<inp.length();i++,k/=16){
		int d=(int)inp[i];
		if(d >= (int)'A'){
			d =d-(int)'A'+10;
		} else d-=(int)'0';
		m+=d*k;
	}
	while(m>0){
		ret= (char)(m%2 + '0')+ret;
		m/=2;
	}
	while(ret.length()<8)ret= '0' + ret;
	return ret;
}
 
string decrypt(string inp){
	string temp="",ret="",inp1="",inp2="";
	for(int i=0;i<inp.length();i+=2){
		temp=temp + inp[i] + inp[i+1];
		inp1 = inp1 + hexaToBit(temp);
		temp="";
	}
	for(int i=0;i<inp1.length();i++){
		inp2+=inp1[i];
		if(inp2.length()==64){
			ret=ret+enc(inp2);
			inp2="";
		}
	}
	cout<<ret.length()<<"\n";
	return ret;
}
 
int main(){
	initTable();	
 
	cout << "Masukkan\n1 untuk ENCRYPT\n2 untuk DECRYPT with known KEY\n";
	cout << "Masukkan Pilihan : ";
	getline(cin,n);	
 
	inputKey();
	buildCD();
	buildK();
 
	if(n[0]=='1') {
		string inp;
		cout<<"Masukkan plaintext : ";getline(cin,inp);
		while(inp.length()%8!=0){
			inp = inp + ' ';
		}
		string ans = encrypt(inp); 
		cout<<"Chipertext : "<<ans<<"\n";
	} else 
	if(n[0]=='2'){
		string inp;
		cout<<"Masukkan chipertext (DALAM BENTUK HEXADECIMAL DENGAN HURUF KAPITAL): "; getline(cin,inp);
		for(int i=1;i<=8;i++){
			for(int j=0;j<48;j++){
				int temp=K[i][j];
				K[i][j]=K[17-i][j];
				K[17-i][j]=temp;
			}
		}
		string ans = decrypt(inp);
		cout<<"Plaintext : "<<ans<<"\n";
	}
	cout<<"\n\nFum is Crypto\n";
}
