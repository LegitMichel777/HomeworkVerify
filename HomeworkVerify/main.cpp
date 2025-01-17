#define CANUSEFS 1
#define APPL 0
#define WIN 1
#define SYS 0
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <set>
#include <signal.h>
#include "uncstr.h"
#if CANUSEFS
#include <filesystem>
#endif
#include "aes.h"
#include "hwfx.h"
#include "common_func.h"
#if SYS==APPL
#include "resources/openhelperm.h"
#include "resources/gpphelperm.h"
#include "resources/hkillm.h"
#elif SYS==WIN
#include "resources/openhelperw.cpp"
#include "resources/gpphelperw.cpp"
#include "resources/hkillw.cpp"
#endif

#if SYS==APPL
#include "handclap.h"
#include "highonlife.h"
#include "acsound.h"
#include "irrlib.h"
#include "soundsetup.h"
#endif

#if SYS==WIN
#include <Shlobj.h>
#endif
using namespace std;
#if CANUSEFS
namespace fs = std::__fs::filesystem;
#endif
typedef long long ll;
typedef long double ld;
//MARK:START UTILITIES
struct compilerInfo {
    string pth;
    string alias;
    string compForm;
};
#if SYS==APPL
string fname = getlogin();
#endif
#if SYS==WIN
string fname;
#endif
string db;
//MARK:SYSTEM CALLS
void inAppLaunch(string s,string args,int OS=SYS) {
    if (OS==APPL) {
        if (args=="") system((safespace(s)).c_str());
        else system((safespace(s)+" "+args).c_str());
    } else if (OS==WIN) {
        if (args=="") system((safespace(s)).c_str());
        else system((safespace(s)+" "+args).c_str());
    }
}
void stopProcess(string processName) {
    inAppLaunch(db+"hkill",processName);
}
void launchNewProcess(string s,int OS=SYS) {
    if (OS==APPL) {
        system(("open -a Terminal "+safespace(s)).c_str());
    } else if (OS==WIN) {
        system(("start \"\" "+safespace(s)).c_str());
    }
}
void clc(int OS=SYS) {
    if (OS==APPL) {
        system("clear");
    } else if (OS==WIN) {
        system("cls");
    }
}
void makeDir(string pathWithSlash) {
#if CANUSEFS
    fs::create_directory(pathWithSlash);
#else
    system(("mkdir "+safespace(pathWithSlash)).c_str());
#endif
}
void removeWithinFolder(string pathWithSlash,int OS=SYS) {
#if CANUSEFS
    fs::remove_all(pathWithSlash);
    fs::create_directory(pathWithSlash);
#else
    if (OS==APPL) {
        system(("rm -r "+safespace(pathWithSlash)+"*").c_str());
    } else if (OS==WIN) {
        system(("rd /S /Q "+safespace(pathWithSlash)).c_str());
        makeDir(pathWithSlash);
    }
#endif
}
void makeExe(string pth,int OS=SYS) {
    if (OS==APPL) {
#if CANUSEFS
        fs::permissions(pth,fs::perms::all);
#else
        system(("chmod 755 "+safespace(pth)).c_str());
#endif
    } else if (OS==WIN) {
        //do nothing
    }
}
void removeFolder(string pathWithSlash,int OS=SYS) {
#if CANUSEFS
    fs::remove_all(pathWithSlash);
#else
    if (OS==APPL) {
        system(("rm -r "+safespace(pathWithSlash)).c_str());
    } else if (OS==WIN) {
        system(("rd /S /Q "+safespace(pathWithSlash)).c_str());
    }
#endif
}
void copyf(string from, string to, bool ovrwrite,int OS=SYS) {
#if CANUSEFS
    ifstream checkifex(to);
    if (!checkifex.good()) {
        checkifex.close();
        fs::copy(from,to);
    } else if (ovrwrite) {
        checkifex.close();
        remove(to.c_str());
        fs::copy(from,to);
    }
#else
    if (OS==APPL) {
        system(("cp "+safespace(from)+" "+safespace(to)).c_str());
    } else if (OS==WIN) {
        system(("copy /y "+safespace(from)+" "+safespace(to)+" >nul 2>nul").c_str());
    }
#endif
}
//MARK:INSTALL
void install(bool comple) {
    cout<<"Installing neccessary components... ";
    if (comple) {
        makeDir(db);
        makeDir(db+"tmp/");
        makeDir(db+"tmp2/");
        ofstream disExist(db+"tmp2/exists");
    }
    //MAC:Install gpphelper(106048),openhelper(203792),handclap(1818048),highonlife(1160862),acsound(94848),hkill(50864),libirrklang.dylib(1975552),soundsetup(114720),convtool(109344)
    //WIN:Install openhelper(1957744),gpphelper(1936998),hkill(1926234)
    string pstfx="";
    if (SYS==WIN) pstfx=".exe";
    ofstream installer;
    const unsigned char* tmp;
    
    installer.open(db+"gpphelper"+pstfx,ios::binary);
#if SYS==APPL
    tmp = gpphelperm();
#elif SYS==WIN
    tmp=gpphelperw();
#endif
    if (SYS==APPL) for (ll i=0;i<138528;i++) installer<<tmp[i];
    else if (SYS==WIN) for (ll i=0;i<1936998;i++) installer<<tmp[i];
    installer.close();
    makeExe(db+"gpphelper"+pstfx);
    
    installer.open(db+"openhelper"+pstfx,ios::binary);
#if SYS==APPL
    tmp = openhelperm();
#elif SYS==WIN
    tmp=openhelperw();
#endif
    if (SYS==APPL) for (ll i=0;i<203792;i++) installer<<tmp[i];
    else if (SYS==WIN) for (ll i=0;i<1957744;i++) installer<<tmp[i];
    installer.close();
    makeExe(db+"openhelper"+pstfx);
    
    installer.open(db+"hkill"+pstfx,ios::binary);
#if SYS==APPL
    tmp = hkillm();
#elif SYS==WIN
    tmp=hkillw();
#endif
    if (SYS==APPL) for (ll i=0;i<50864;i++) installer<<tmp[i];
    else if (SYS==WIN) for (ll i=0;i<1926234;i++) installer<<tmp[i];
    installer.close();
    makeExe(db+"hkill"+pstfx);
    
#if SYS==APPL
    installer.open(db+"U2PQX4CHY3SOJ31E.wav",ios::binary);
    tmp=handclap();
    for (ll i=0;i<1818048;i++) installer<<tmp[i];
    installer.close();
    
    installer.open(db+"D63SR3HEXSB6JX1N.wav",ios::binary);
    tmp=highonlife();
    for (ll i=0;i<1160862;i++) installer<<tmp[i];
    installer.close();
    
    installer.open(db+"acsound"+pstfx,ios::binary);
    tmp = acsound();
    for (ll i=0;i<94848;i++) installer<<tmp[i];
    installer.close();
    makeExe(db+"acsound"+pstfx);
    
    installer.open(db+"soundsetup"+pstfx,ios::binary);
    tmp=soundsetup();
    for (ll i=0;i<114720;i++) installer<<tmp[i];
    installer.close();
    makeExe(db+"soundsetup"+pstfx);
    
    installer.open(db+"libirrklang.dylib",ios::binary);
    tmp=irrlib();
    for (ll i=0;i<1975552;i++) installer<<tmp[i];
    installer.close();
    
    cout<<"done"<<endl;
#endif
}
bool fcopy(string from,string to) {
    ifstream in(from);
    ofstream out(to);
    string s;
    if (in.good()&&out.good()) {
        out<<in.rdbuf();
    } else {
        in.close();
        out.close();
        return 1;
    }
    in.close();
    out.close();
    return 0;
}
void enterToCont() {
    cout<<"Press enter to continue..."<<endl;
    cin.get();
}
string key = "FB384BE6E7009275";
string iv = "755CA3572C3FAC78";
string salt = "8C55A67258E39056";
string cmdenc = "openssl enc -aes-256-cbc -K "+key+" -iv "+iv+" -S "+salt;
//MARK:END UTILITIES
ll srtmode,evalLang,mltcore,rten,compcore,cor=0,incor=0,helperver,sound,autosave,ovrflw,anslock,fsvers,expfsvers,prefersCompiler,debugMd,settingshelp,newusrcompile,unicodehandl;
string lockedf,vers,expvers,preferredCompiler,compilerFormat;
unsigned char aesKey[32]={28,57,67,10,127,108,14,197,41,73,79,16,242,2,85,227,120,13,53,121,23,109,231,129,210,147,11,128,115,1,242,150};
map<ll,char>randname;
bool cmp(hw const &a, hw const &b) {
    if (srtmode==1) return a.name<b.name;
    else if (srtmode==2) return a.name>b.name;
    else if (srtmode==3) {
        if (a.score==b.score) return a.name<b.name;
        return a.score<b.score;
    }
    else if (srtmode==4) {
        if (a.score==b.score) return a.name<b.name;
        return a.score>b.score;
    }
    else if (srtmode==5||srtmode==6) {
        if (a.date==b.date) {
            return a.name<b.name;
        }
        ll t1,t2;
        t1=strtm2int(a.date);
        t2=strtm2int(b.date);
        if (t1-t2>0) {
            if (srtmode==5) return false;
            else return true;
        } else {
            if (srtmode==6) return true;
            else return false;
        }
    }
    return 0;
}
vector<hw>asmt;
struct egg {
    string ach, trig, msg;
};
vector<egg>easter;
map<string,ll*>intcont;
map<string,string*>strcont;
map<string,vector<string>>structref;
struct soundlog {
    string fname,sname;
};
struct dtCont {
    ll cnt;
    ll ipos,opos;
};
bool* secret;
vector<soundlog>audio;
vector<ll>selaud;
struct strWithInt {
    string ply;
    ll mtd;
};
void savedata() {
    ofstream out(db+"contents.dwt",ios::binary);
    if (!out.good()) {
        cout<<"Critical error! Error accessing contents.dwt save file! Path:"<<db+"contents.dwt"<<endl;
        enterToCont();
    }
    out<<"ACH"<<endl;
    for (ll i=0;i<easter.size();i++) out<<secret[i];
    out<<endl;
    for (map<string,ll*>::iterator it=intcont.begin();it!=intcont.end();it++) out<<it->first<<endl<<*it->second<<endl;
    for (map<string,string*>::iterator it=strcont.begin();it!=strcont.end();it++) out<<it->first<<endl<<*it->second<<endl;
    out<<"dwtReserved::STARTSTRUCT"<<endl<<"Hwf"<<endl<<"FNAME"<<endl<<"SCORE"<<endl<<"NAME"<<endl<<"DATE"<<endl<<"dwtReserved::ENDSTRUCT"<<endl<<"dwtReserved::STARTSTRUCT"<<endl<<"Song"<<endl<<"FNAME"<<endl<<"SNAME"<<endl<<"dwtReserved::ENDSTRUCT"<<endl<<"dwtReserved::STARTSTRUCT"<<endl<<"selaud"<<endl<<"AUDID"<<endl<<"dwtReserved::ENDSTRUCT"<<endl<<"dwtReserved::START"<<endl<<"Song"<<endl;
    for (ll i=0;i<audio.size();i++) out<<audio[i].fname<<endl<<audio[i].sname<<endl;
    out<<"dwtReserved::END"<<endl<<"Song"<<endl<<"dwtReserved::START"<<endl<<"Hwf";
    for (ll i=0;i<asmt.size();i++) out<<endl<<asmt[i].id<<endl<<asmt[i].score<<endl<<asmt[i].name<<endl<<asmt[i].date;
    out<<endl<<"dwtReserved::END"<<endl<<"Hwf"<<endl<<"dwtReserved::START"<<endl<<"selaud";
    for (ll i=0;i<selaud.size();i++) out<<endl<<selaud[i];
    out<<endl<<"dwtReserved::END"<<endl<<"selaud";
    out.close();
}
void nml(string &s) {
    if (SYS==APPL) {
        if (s!="") {
            //any special character not escaped->hand typed
            if (s[s.size()-1]=='\\') return;
            string rturn="";
            map<char,bool>toEscape;
            toEscape['~']=toEscape['=']=toEscape['[']=toEscape[']']=toEscape['{']=toEscape['}']=toEscape['\\']=toEscape['|']=toEscape[';']=toEscape['\'']=toEscape['"']=toEscape['<']=toEscape['>']=toEscape['?']=toEscape[' ']=toEscape['!']=toEscape['#']=toEscape['%']=toEscape['&']=toEscape['*']=toEscape['(']=toEscape[')']=1;
            for (ll i=0;i<s.size()-1;i++) {
                if (s[i]=='\\'&&toEscape[s[i+1]]) {
                    rturn+=s[++i];
                } else rturn+=s[i];
            }
            rturn+=s[s.size()-1];
            for (ll i=0;i<rturn.size();i++) {
                if (rturn[i]==':') rturn[i]='/';
            }
            if (rturn[rturn.size()-1]==' ') rturn.erase(rturn.size()-1);
            s=rturn;
        }
    } else if (SYS==WIN) {
        if (s!="") {
            if (s[0]=='"') {
                s.erase(s.begin());
            }
            if (s[s.size()-1]=='"') {
                s.erase(s.begin()+s.size()-1);
            }
        }
    }
}
void changelog() {
    cout<<"What's changed in CodeAssign Phoenix:"<<endl<<"Lots of internal improvements and tweaks"<<endl;
}
//MARK:MAIN
map<ll,string>uncreplc;
ll pkgImport(ifstream &rdpkg, string &newPkg, string impItm,vector<string> &errorlog,bool treatunc) {
    if (unicodehandl==1) treatunc=false;
    stringstream checkNef;
    if (rdpkg.good()) {
        checkNef<<rdpkg.rdbuf();
        if (treatunc) {
            uncstr nef(checkNef.str());
            if (nef.find("HwfxContents::")==string::npos) {
                //check for unicode
                for (ll i=0;i<nef.size();i++) {
                    if (nef.cval(i)>=128) {
                        //unicodehandl = 0: attempt to replace every valid unicode value, call errors on any non-unicode values
                        //=1: keep unicode as is
                        //=2: attempt to replace every valid unicode value but don't call errors
                        //                    errorlog.push_back("Unicode value "+to_string(nef.cval(i))+" detected");
                        if (unicodehandl==0||unicodehandl==2) {
                            if (uncreplc.find(nef.cval(i))!=uncreplc.end()) {
                                nef.replace(i,uncreplc[nef.cval(i)]);
                            } else if (unicodehandl==0) {
                                errorlog.push_back("Unexpected Unicode value "+to_string(nef.cval(i))+" in "+impItm);
                            }
                        }
                    }
                }
                newPkg+=nef.toS()+'\n';
            } else {
                errorlog.push_back("Import item "+impItm+" contains keyword \"HwfxContents::\"!");
                return -1;
            }
        } else {
            string nef=checkNef.str();
            if (nef.find("HwfxContents::")==string::npos) {
                newPkg+=nef+'\n';
            } else {
                errorlog.push_back("Import item "+impItm+" contains keyword \"HwfxContents::\"!");
                return -1;
            }
        }
    } else {
        errorlog.push_back("Item "+impItm+" is missing!");
        return -1;
    }
    return 0;
}
void writeHwfx(string &pkgpth,bool &impFail,string &disTtl,string &newPkg,vector<string> &errorlog) {
    impFail=false;
    newPkg="HwfxContents::Payload\nHwfxContents::StartCache\nHwfxContents::Title\n";
    ifstream rdpkg(pkgpth+"/title.txt",ios::binary);
    if (pkgImport(rdpkg,newPkg,"Title",errorlog,true)==-1) impFail=true;
    rdpkg.close();
    rdpkg.open(pkgpth+"/title.txt");
    if (rdpkg.good()) getline(rdpkg,disTtl);
    rdpkg.close();
    newPkg+="HwfxContents::Description\n";
    rdpkg.open(pkgpth+"/description.txt");
    if (pkgImport(rdpkg,newPkg,"Description",errorlog,true)==-1) impFail=true;
    rdpkg.close();
    newPkg+="HwfxContents::InputDes\n";
    rdpkg.open(pkgpth+"/in.txt");
    if (pkgImport(rdpkg,newPkg,"Input",errorlog,true)==-1) impFail=true;
    rdpkg.close();
    newPkg+="HwfxContents::OutputDes\n";
    rdpkg.open(pkgpth+"/out.txt");
    if (pkgImport(rdpkg,newPkg, "Output",errorlog,true)==-1) impFail=true;
    rdpkg.close();
    ll expkgs=1;
    while (true) {
        ifstream intst(pkgpth+"/exin"+to_string(expkgs)+".txt"),outtst(pkgpth+"/exout"+to_string(expkgs)+".txt");
        if (intst.good()&&outtst.good()) {
            newPkg+="HwfxContents::EXIN"+to_string(expkgs)+"\n";
            if (pkgImport(intst,newPkg,"Example input "+to_string(expkgs),errorlog,false)==-1) {
                impFail=true;
                break;
            }
            newPkg+="HwfxContents::EXOU"+to_string(expkgs)+"\n";
            if (pkgImport(outtst,newPkg,"Example output "+to_string(expkgs),errorlog,false)==-1) {
                impFail=true;
                break;
            }
            expkgs++;
        } else if (intst.good()||outtst.good()) {
            if (intst.good()) errorlog.push_back("MISMATCH_EXOUT_GONE ("+to_string(expkgs)+".txt, "+to_string(expkgs)+".txt)");
            else errorlog.push_back("MISMATCH_EXIN_GONE ("+to_string(expkgs)+".txt, "+to_string(expkgs)+".txt");
            impFail=true;
            expkgs--;
            break;
        } else {
            expkgs--;
            break;
        }
    }
    newPkg+="HwfxContents::EndCache\n";
    ll testSets=1;
    while (true) {
        ifstream intst(pkgpth+"/in"+to_string(testSets)+".txt"),outtst(pkgpth+"/out"+to_string(testSets)+".txt");
        if (intst.good()&&outtst.good()) {
            newPkg+="HwfxContents::INST"+to_string(testSets)+'\n';
            if (pkgImport(intst,newPkg,"Input "+to_string(testSets),errorlog,false)==-1) {
                impFail=true;
                break;
            }
            newPkg+="HwfxContents::OUST"+to_string(testSets)+'\n';
            if (pkgImport(outtst,newPkg,"Output "+to_string(testSets),errorlog,false)==-1) {
                impFail=true;
                break;
            }
            testSets++;
        } else if (intst.good()||outtst.good()) {
            if (intst.good()) errorlog.push_back("MISMATCH_OUT_GONE ("+to_string(testSets)+".txt, "+to_string(testSets)+".txt)");
            else errorlog.push_back("MISMATCH_IN_GONE ("+to_string(testSets)+".txt, "+to_string(testSets)+".txt)");
            impFail=true;
            testSets--;
            break;
        } else {
            testSets--;
            break;
        }
    }
}
void cleanStr(string &s) {
    ll sl=0;
    for (ll i=s.size()-1;i>=0;i--,sl++) {
        if (s[i]!='\n'&&s[i]!='\r') break;
    }
    s.erase(s.begin()+s.size()-sl,s.end());
}

void testAes(ll tstmulcnt,ll tstmulsz,ll tstmulthr,ll tstkeysz) {
    cout<<"Testing "<<tstmulcnt<<" times, each time with "<<tstmulsz<<" bytes of data. Running on "<<tstmulthr<<" thread";
    if (tstmulthr>1) cout<<"s."<<endl;
    else cout<<"."<<endl;
    cout<<"Generating data..."<<endl;
    unsigned char *testData=new(nothrow) unsigned char[tstmulsz*tstmulcnt];
    if (testData) {
        for (ll i=0;i<tstmulcnt*tstmulsz;i++) testData[i]=rand()%256;
    } else {
        cout<<"Memory allocation failed for test data."<<endl;
        return;
    }
    cout<<"Generating passwords and IV"<<endl;
    vector<unsigned char *>iv[tstmulcnt];
    for (ll i=0;i<tstmulcnt;i++) {
        for (ll j=0;j<tstmulthr;j++) {
            unsigned char* distmp=new unsigned char[8];
            if (!distmp) {
                cout<<"Memory allocation failed for iv "<<i<<' '<<j<<endl;
                delete[] testData;
                for (ll k=0;k<tstmulcnt;k++) for (ll l=0;l<iv[k].size();l++) if (iv[k][l]) delete[] iv[k][l];
                return;
            }
            for (ll k=0;k<8;k++) distmp[k]=rand()%256;
            iv[i].push_back(distmp);
        }
    }
    unsigned char *key=new unsigned char[tstmulcnt*tstkeysz];
    if (!key) {
        cout<<"Memory allocation failed for keys"<<endl;
        for (ll i=0;i<tstmulcnt;i++) for (ll j=0;j<iv[i].size();j++) delete[] iv[i][j];
        delete[] testData;
    }
    for (ll i=0;i<tstmulcnt;i++) {
        for (ll j=0;j<tstkeysz;j++) {
            key[i*tstkeysz+j]=rand()%256;
        }
    }
    cout<<"Copying data..."<<endl;
    unsigned char *toEncData=new(nothrow) unsigned char[tstmulcnt*tstmulsz];
    if (toEncData) {
        for (ll i=0;i<tstmulcnt*tstmulsz;i++) toEncData[i]=testData[i];
    } else {
        cout<<"Memory allocation failed for encryption data"<<endl;
        for (ll i=0;i<tstmulcnt;i++) for (ll j=0;j<iv[i].size();j++) delete[] iv[i][j];
        delete[] testData;
    }
    cout<<"Encrypting..."<<endl;
    chrono::steady_clock::time_point grs=chrono::steady_clock::now();
    for (ll i=0;i<tstmulcnt;i++) {
        aes_encrypt(toEncData+i*tstmulsz, tstmulsz, key+i*tstkeysz, tstkeysz, iv[i]);
    }
    cout<<"Encryption done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
    cout<<"Decrypting..."<<endl;
    grs=chrono::steady_clock::now();
    for (ll i=0;i<tstmulcnt;i++) {
        aes_decrypt(toEncData+i*tstmulsz,tstmulsz, key+i*tstkeysz, tstkeysz, iv[i]);
    }
    cout<<"Decryption done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
    cout<<"Matching data..."<<endl;
    bool issue=false;
    for (ll i=0;i<tstmulcnt*tstmulsz;i++) {
        if (toEncData[i]!=testData[i]) {
            cout<<"Error at test #"<<i/tstmulsz+1<<", byte #"<<i%tstmulsz<<endl;
            issue=true;
        }
    }
    if (issue) cout<<"Passed with 0 issues."<<endl;
    delete[] toEncData;
    for (ll i=0;i<tstmulcnt;i++) for (ll j=0;j<iv[i].size();j++) delete[] iv[i][j];
    delete[] testData;
}
void setupCompiler(ll &newUsr,bool &compilerExists,vector<compilerInfo>&compilers,bool &compilerVirgin) {
    while (!compilerExists) {
        ifstream testCompiler;
        string compilerPath;
        if (prefersCompiler) {
            compilerPath=preferredCompiler;
            testCompiler.open(compilerPath);
            if (testCompiler.good()) compilerExists=true;
            else compilerExists=false;
            testCompiler.close();
        } else {
            if (preferredCompiler!="") {
                ll foundPref=-1;
                for (ll i=0;i<compilers.size()&&foundPref==-1;i++) if (preferredCompiler==compilers[i].alias) foundPref=i;
                if (foundPref!=-1) {
                    ifstream testDis(compilers[foundPref].pth);
                    compilerExists=testDis.good();
                    testDis.close();
                } else {
                    preferredCompiler="";
                    cout<<"Invalid compiler ID!"<<endl;
                    continue;
                }
            } else {
                for (ll i=0;i<compilers.size();i++) {
                    ifstream testCompiler(compilers[i].pth);
                    if (testCompiler.good()) {
                        preferredCompiler=compilers[i].alias;
                        compilerExists=true;
                        testCompiler.close();
                        break;
                    }
                }
            }
        }
        
        if (!compilerVirgin&&compilerExists&&prefersCompiler) { //set a new compiler
            cout<<"Compiler successfully found."<<endl;
            if (prefersCompiler) {
                while (true) {
                    cout<<"Current compiler format:"<<compilerFormat<<endl<<"Is this correct?"<<endl<<"[1]Yes"<<endl<<"[2]No"<<endl;
                    string formatcor;
                    getline(cin,formatcor);
                    if (formatcor=="1") {
                        break;
                    } else if (formatcor=="2") {
                        clc();
                        bool masterBreak=false;
                        while (true) {
                            cout<<"Please input the new format. Input exit to cancel."<<endl;
                            string tmpcompilerFormat;
                            getline(cin,tmpcompilerFormat);
                            string exitTry=tmpcompilerFormat;
                            transform(exitTry.begin(),exitTry.end(),exitTry.begin(),::tolower);
                            if (exitTry=="exit"){
                                clc();
                                masterBreak=true;
                                break;
                            }
                            if (tmpcompilerFormat.find("%CODEPATH%")!=string::npos&&tmpcompilerFormat.find("%EXECPATH")!=string::npos) {
                                compilerFormat=tmpcompilerFormat;
                                clc();
                                masterBreak=true;
                                break;
                            } else {
                                clc();
                                cout<<"Error! Compiler format must include %CODEPATH% (The path to the code) and %EXECPATH% (The path to the output executable)!"<<endl;
                            }
                        }
                        if (masterBreak) break;
                    }
                }
            }
        }
        
        if (compilerExists) break;
        if (newUsr) {
            cout<<"A compiler cannot be found. A compiler allows you and CodeAssign to execute code."<<endl;
            if (SYS==APPL) cout<<"To install a compiler, open the terminal and type in \"xcode-select --install\".";
            else if (SYS==WIN) cout<<"To install a compiler, you can install a supported IDE, like Code::Blocks(This compiler may not work with CodeAssign) and Dev-C++(recommended).";
            cout<<" Once installed, CodeAssign will detect the compiler automatically."<<endl;
            cout<<"You can:"<<endl;
        } else {
            cout<<"Error! Compiler not found!"<<endl;
        }
        string compAnsr;
        if (prefersCompiler) {
            ll nxtFnd=-1;
            for (ll i=0;i<compilers.size();i++) {
                ifstream testIfGoodComp(compilers[i].pth);
                if (testIfGoodComp.good()) {
                    nxtFnd=i;
                    testIfGoodComp.close();
                    break;
                }
            }
            if (nxtFnd!=-1) {
                testCompiler.close();
                cout<<"Your custom compiler ("<<preferredCompiler<<") does not exist but the default "<<compilers[nxtFnd].alias<<" compiler does."<<endl<<"[1]Try again with my compiler"<<endl<<"[2]Use CodeAssign's default compiler"<<endl<<"[3]Set new custom compiler"<<endl<<"[4]Use CodeAssign without evaluation features"<<endl;
                getline(cin,compAnsr);
                if (compAnsr=="2") {
                    prefersCompiler=0;
                    preferredCompiler=compilers[nxtFnd].alias;
                    compilerFormat=compilers[nxtFnd].compForm;
                } else if (compAnsr=="3") {
                    clc();
                    cout<<"Please drag the desired compiler into this window."<<endl;
                    getline(cin,preferredCompiler);
                    nml(preferredCompiler);
                } else if (compAnsr=="4") {
                    clc();
                    break;
                } else if (compAnsr!="1") cout<<"Invalid response."<<endl;
            } else {
                cout<<"Your custom compiler ("<<preferredCompiler<<") does not exist and CodeAssign cannot find it's default ";
                if (compilers.size()>=3) {
                    for (ll i=0;i<compilers.size()-1;i++) cout<<compilers[i].alias<<", ";
                    cout<<"and"<<compilers[compilers.size()-1].alias;
                } else if (compilers.size()==2) {
                    cout<<compilers[0].alias<<" and "<<compilers[1].alias;
                } else {
                    cout<<compilers[0].alias;
                }
                cout<<" compiler either."<<endl<<"[1]Try again"<<endl<<"[2]Set a new custom compiler"<<endl<<"[3]Try to use the default compiler";
                if (compilers.size()>1) cout<<"s";
                cout<<" and try again"<<endl<<"[4]Use CodeAssign without evaluation features"<<endl;
                getline(cin,compAnsr);
                if (compAnsr=="2") {
                    clc();
                    cout<<"Please drag the desired compiler into this window."<<endl;
                    getline(cin,preferredCompiler);
                    nml(preferredCompiler);
                } else if (compAnsr=="3") {
                    prefersCompiler=0;
                    preferredCompiler="";
                } else if (compAnsr=="4") {
                    clc();
                    break;
                } else if (compAnsr!="1") cout<<"Invalid response."<<endl;
            }
        } else {
            //try other default compilers and if they do work, use them.
            ll nxtFnd=-1;
            for (ll i=0;i<compilers.size();i++) {
                ifstream testIfGoodComp(compilers[i].pth);
                if (testIfGoodComp.good()) {
                    nxtFnd=i;
                    break;
                }
            }
            if (nxtFnd==-1) {
                if (newUsr) {
                    cout<<"[1]Try detecting a compiler again."<<endl<<"[2]Set a custom compiler (Advanced)"<<endl<<"[3]Set up a compiler later and use CodeAssign in read only mode"<<endl;
                } else {
                    cout<<"CodeAssign's default ";
                    if (SYS==APPL) cout<<"Xcode";
                    else if (SYS==WIN) cout<<"Dev-C++ and Code::Blocks";
                    cout<<" compiler cannot be found."<<endl;
                    if (SYS==APPL) cout<<"To install the Xcode Clang compiler, open Terminal and type in \"xcode-select --install\"."<<endl;
                    else if (SYS==WIN) cout<<"Please install Dev-C++(recommended), Code::Blocks(This compiler may not work with CodeAssign), or set your own custom compiler."<<endl;
                    cout<<"If you do have a compiler on the system, set it as a custom compiler and report it so CodeAssign can detect it in future versions!"<<endl;
                    cout<<"[1]Try again"<<endl<<"[2]Set a custom compiler"<<endl<<"[3]Use CodeAssign without evaluation features"<<endl;
                }
                getline(cin,compAnsr);
                if (compAnsr=="2") {
                    clc();
                    cout<<"Please drag the desired compiler into this window."<<endl;
                    getline(cin,preferredCompiler);
                    nml(preferredCompiler);
                    prefersCompiler=1;
                } else if (compAnsr=="3") {
                    clc();
                    break;
                } else if (compAnsr!="1") cout<<"Invalid response."<<endl;
            }
        }
        clc();
        compilerVirgin=false;
    }
    if (compilerExists) newUsr=false;
}
int main() {
    bool winDirDectFail=false;
    #if SYS==WIN
    WCHAR tmppathvar[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL,CSIDL_PROFILE,NULL,0,tmppathvar))) {
        wstring tmppathstr(tmppathvar);
        fname=string(tmppathstr.begin(),tmppathstr.end());
    } else {
        winDirDectFail=true;
    }
    #endif
    string exePost="";
    if (SYS==WIN) exePost=".exe";
    //TODO:Add batch evaluation for fs
    //TODO:Add search
    intcont["SOUND"]=&sound;
    intcont["CORRECT"]=&cor;
    intcont["INCORRECT"]=&incor;
    intcont["SORT"]=&srtmode;
    intcont["MULTICORE"]=&mltcore;
    intcont["RETENTION"]=&rten;
    intcont["AUTOSAVE"]=&autosave;
    intcont["HELPERVER"]=&helperver;
    intcont["OVRFLW"]=&ovrflw;
    intcont["ANSLOCK"]=&anslock;
    intcont["FSVERS"]=&fsvers;
    intcont["HASPREFERREDCOMPILER"]=&prefersCompiler;
    intcont["DEBUG"]=&debugMd;
    intcont["SETTINGSHELP"]=&settingshelp;
    intcont["NEWUSRCOMPILE"]=&newusrcompile;
    intcont["UNCSET"]=&unicodehandl;
    strcont["LOCKEDF"]=&lockedf;
    strcont["VERS"]=&vers;
    strcont["PREFERREDCOMPILER"]=&preferredCompiler;
    strcont["COMPILERFORMAT"]=&compilerFormat;
    ll const helpertar=6;
    unicodehandl=0;
    rten=6;
    mltcore=1;
    srtmode=1;
    incor=cor=0;
    sound=0;
    autosave=1;
    ovrflw=0;
    expfsvers=2;
    expvers="1.5.7";
    debugMd=0;
    compilerFormat="%CODEPATH% -o %EXECPATH%";
    preferredCompiler="";
    audio.push_back({"U2PQX4CHY3SOJ31E","Handclap"});
    audio.push_back({"D63SR3HEXSB6JX1N","High on Life"});
    selaud.push_back(0);
    selaud.push_back(1);
    if (true) /*Easter eggs!*/ {
        easter.push_back({"The Player's Poem","player","Roses are red, \nViolets are blue.\nI said I loved you,\nYou thought that was true.\nWell guess what player?\nYou just got played too."});
        easter.push_back({"?ldroW","hello world","!dlroW olleH"});
        easter.push_back({"Secrets...","easter egg","Secrets have been hidden..."});
        easter.push_back({"Demons","horn","Try not to get me wet"});
        easter.push_back({"ME!","credits","CODEASSIGN MADE BY MICHEL!"});
        easter.push_back({"Ding!","tiger","Buy the Tiger Microwave and the Ding Radio Emitter!"});
        easter.push_back({"Always On!","eric","That man must be turned off"});
        easter.push_back({"You ded!","msyan","For Ms.Yan..."});
        easter.push_back({"Respects","f","You've paid your respects."});
        easter.push_back({"You kiss your mother with that mouth?","fack","If you see kay, why oh you?"});
        easter.push_back({"My name!","michel","That's me!"});
        easter.push_back({"Language of the future...","c++","That's the language!"});
        easter.push_back({"MINE! MINE!","lucy","Do your problems... and come talk to me. Tell me that I sent you"});
        easter.push_back({"Cat Spring","cucumber","Cats like them... I think"});
        easter.push_back({"Best subject","math","Awesome Math!"});
        easter.push_back({"Slap his thigh!","lightning","Lightning and the thunder..."});
        easter.push_back({"I'll do you every day.","homework","Oh yeah..."});
        easter.push_back({"Goose math!","3.16","Pi IS 3.16!"});
        easter.push_back({"Escaping red blood cells!","bleed","Rudolph's running nose..."});
        easter.push_back({"The mythical creature","unicorn","So horny..."});
        easter.push_back({"Greatest club of all time","c++ programming","Yee!"});
        easter.push_back({"Best calculator","calculatorx","Powerful."});
        easter.push_back({"VB sucks like VB","vb","One who sucks like VB"});
        easter.push_back({"Be careful...","hairline","Look after yourself."});
        easter.push_back({"Elon's fail","cybertruck","SMASH!"});
        easter.push_back({"+x","martin garrix","PIZZA!"});
        easter.push_back({"The precursors...","c","(C++)--"});
        easter.push_back({"UPDAAATTTEEEE","windows","Update and restart? Please?"});
        easter.push_back({"The Answer","42","The Answer to Life, the Universe and Everything"});
        easter.push_back({"LOL","lolcode","VISIBLE \"Hello World!\""});
        easter.push_back({"Your next teacher...","codeassign","Your next teacher is not a teacher."});
    }
    uncreplc[8230]="...";
    uncreplc[8805]=">=";
    uncreplc[8722]="--";
    uncreplc[8800]="!=";
    uncreplc[215]="x";
    uncreplc[8217]="'";
    uncreplc[8242]="'";
    uncreplc[8901]="x";
    uncreplc[8943]="...";
    uncreplc[8804]="<=";
    //Convert old compiler format to new compiler format
    if (compilerFormat=="") compilerFormat="%CODEPATH% -o %EXECPATH%";
    //End convo
    
    secret=new bool[easter.size()];
    vector<string>tips;
    if (true) {
        tips.push_back("Type make into the main menu to create your own problems!");
        tips.push_back("If you have too many files, consider using keep problems and limit items in settings!");
        tips.push_back("Try setting multiple threads in settings to evaluate your problems faster!");
        tips.push_back("Type changelog into the main menu to view the changelog!");
        tips.push_back("Type the name of a question into the main menu!");
        tips.push_back("If you're having errors, try typing \"hreset\" into the main menu!");
        tips.push_back("Visit C++ Programming Club on SHSID Connect! https://connect.shs.cn/club/338_C++_Programming_Club/");
        if (SYS==APPL) {
            tips.push_back("If the window doesn't look nice or the text is too small, try going to Terminal->Preferences->Profiles, and customising some settings! Alternatively, ask for the \"Friendly\" terminal profile from Michel!");
        } else if (SYS==WIN) {
            tips.push_back("If the window doesn't look nice or the text is too small, try right clicking the top of the window, selecting properties, and making some customisations!");
        }
    }
    //openssl enc -aes-256-cbc -K FB384BE6E7009275 -iv 755CA3572C3FAC78 -S 8C55A67258E39056
    clc();
    for (ll i=48;i<=57;i++) {
        randname[i-48]=i;
    }
    for (ll i=65;i<=90;i++) {
        randname[i-55]=i;
    }
    srand(time(0));
    ifstream dbtest;
    dbtest.open("/Users/"+fname);
    if ((!dbtest.good()&&SYS!=WIN)||winDirDectFail) {
        dbtest.close();
        bool isdev=true;
        while (true) {
            if (isdev) {
                cout<<"Automatic directory detection failed. Trying dev legitmichel777..."<<endl;
                fname="legitmichel777";
                isdev=false;
            } else {
                cout<<"Automatic directory detection failed. Please enter your username."<<endl;
                getline(cin,fname);
            }
            dbtest.open("/Users/"+fname);
            if (dbtest.good()) {dbtest.close();break;}
            dbtest.close();
            while (true) {
                cout<<"Error! Please make sure you entered the right user and is on a supported system(";
                if (SYS==APPL) cout<<"macOS";
                else if (SYS==WIN) cout<<"Windows";
                cout<<"). Please enter your username again."<<endl;
                getline(cin,fname);
                dbtest.open("/Users/"+fname);
                if (dbtest.good()) {dbtest.close();break;}
                dbtest.close();
            }
            break;
        }
    }
    dbtest.close();
    if (SYS==APPL) db = "/Users/"+fname+"/Library/Containers/com.mclm7.homework/";
    else db = fname+"/AppData/Roaming/CppHomework/";
    dbtest.open(db+"contents.dwt");
    bool freshins=false;
    bool newUsr=false;
    if (!dbtest.good()) {
        newUsr=true;
        settingshelp=true;
        newusrcompile=true;
        freshins=true;
        install(1);
        ofstream mancreate(db+"contents.dwt");
        mancreate.close();
        helperver=helpertar;
        savedata();
    }
    dbtest.close();
    ifstream in(db+"contents.dwt",ios::binary);
    bool impissue=false;
    bool impfsVers=false;
    ofstream rmsil(db+"tmp/sl.silent");
    if (!rmsil.good()) cout<<"Error writing silent file to tmp"<<endl;
    rmsil<<"SILENCE!";
    rmsil.close();
    removeWithinFolder(db+"tmp/");
    removeWithinFolder(db+"tmp2/");
    ofstream tmp2Exists(db+"tmp2/exists");
    tmp2Exists.close();
    for (ll i=0;i<easter.size();i++) secret[i]=0;
    string prefx;
    if (in.good()&&!freshins) {
        vector<string>contentsf;
        while (!in.eof()) {
            string s;
            getline(in,s);
            s.erase(remove(s.begin(), s.end(), '\r'), s.end());
            contentsf.push_back(s);
        }
        for (ll i=0;i<contentsf.size();i++) {
            string finkey=contentsf[i];
            if (fsvers>=2) prefx="dwtReserved::";
            if (finkey==prefx+"START") {
                bool startgood=false;
                string stname=contentsf[++i];
                for (ll j=i;j<contentsf.size()-1;j++) {
                    if (contentsf[j]==prefx+"END"&&contentsf[j+1]==stname) {
                        startgood=true;
                        break;
                    } else if (contentsf[j]==prefx+"START") break;
                    else if (contentsf[j]==prefx+"END") break;
                }
                if (startgood) {
                    ll cur=0;
                    ll ttl = structref[stname].size();
                    if (ttl) {
                        if (stname=="Hwf") {
                            hw curwt=(hw){"",0,"",""};
                            //ID,NAME,SCORE,DATE
                            while (contentsf[++i]!=prefx+"END") {
                                if (cur==ttl) {
                                    asmt.push_back(curwt);
                                    cur=0;
                                }
                                if (structref[stname][cur]=="FNAME") curwt.id=contentsf[i];
                                else if (structref[stname][cur]=="SCORE") curwt.score=atoll(contentsf[i].c_str());
                                else if (structref[stname][cur]=="NAME") curwt.name=contentsf[i];
                                else if (structref[stname][cur]=="DATE") curwt.date=contentsf[i];
                                cur++;
                            }
                            if (cur==ttl) asmt.push_back(curwt);
                        } else if (stname=="Song") {
                            audio.clear();
                            soundlog curwt=(soundlog){"",""};
                            //fname, sname
                            while (contentsf[++i]!=prefx+"END") {
                                if (cur==ttl) {
                                    audio.push_back(curwt);
                                    cur=0;
                                }
                                if (structref[stname][cur]=="FNAME") curwt.fname=contentsf[i];
                                else if (structref[stname][cur]=="SNAME") curwt.sname=contentsf[i];
                                cur++;
                            }
                            if (cur==ttl) audio.push_back(curwt);
                        } else if (stname=="selaud") {
                            selaud.clear();
                            ll curwt;
                            while (contentsf[++i]!=prefx+"END") {
                                if (cur==ttl) {
                                    selaud.push_back(curwt);
                                    cur=0;
                                }
                                if (structref[stname][cur]=="AUDID") curwt=atoll(contentsf[i].c_str());
                                cur++;
                            }
                            if (cur==ttl) selaud.push_back(curwt);
                        } else while (contentsf[++i]!=prefx+"END");
                    }
                } else impissue=true;
            } else if (finkey==prefx+"STARTSTRUCT") {
                bool structgood=false;
                for (ll j=i+1;j<contentsf.size();j++) {
                    if (contentsf[j]==prefx+"ENDSTRUCT") {
                        structgood=true; //Ending found
                        break;
                    } else if (contentsf[j]==prefx+"STARTSTRUCT") break; //No ending
                }
                if (structgood) {
                    string stuname=contentsf[++i];
                    while (contentsf[++i]!=prefx+"ENDSTRUCT") {
                        structref[stuname].push_back(contentsf[i]);
                    }
                } else impissue=true;
            } else if (intcont.count(finkey)) {
                *intcont[finkey]=atoll(contentsf[++i].c_str());
            } else if (strcont.count(finkey)) {
                *strcont[finkey]=contentsf[++i];
            } else if (finkey=="ACH") {
                i++;
                for (ll j=0;j<min(easter.size(),contentsf[i].size());j++) {
                    if (contentsf[i][j]=='0') secret[j]=0;
                    else if (contentsf[i][j]=='1') secret[j]=1;
                }
            }
        }
    }
    in.close();
    if (vers!=expvers&&!newUsr) {
        if (vers<expvers) {
            cout<<"Welcome to CodeAssign Phoenix!"<<endl;
            changelog();
        }
    }
    ifstream checkTmp2(db+"tmp2/exists");
    if (!checkTmp2.good()) {
        checkTmp2.close();
        makeDir(db+"tmp2/");
        ofstream nowExists(db+"tmp2/exists");
        nowExists<<"I exist!";
        nowExists.close();
    }
    checkTmp2.close();
    if (fsvers!=0&&fsvers!=2) {
        impissue=true;
        impfsVers=true;
    }
    fsvers=2;
    vers=expvers;
    ofstream out(db+"db.txt");
    out<<db;
    out.close();
    if (helperver!=helpertar&&!newUsr) {
        cout<<"Updating helpers..."<<endl;
        install(0);
        helperver=helpertar;
    }
    vector<string>errorlog;
    compcore=thread::hardware_concurrency();
    for (ll i=0;i<asmt.size();i++) {
        ifstream bundltest(db+asmt[i].id);
        if (!bundltest.good()) {
            asmt.erase(asmt.begin()+i);
            i--;
        }
    }
    ll timethres = 0;
    if (rten!=6) {
        if (rten==1) timethres=7;
        if (rten==2) timethres=14;
        if (rten==3) timethres=30;
        if (rten==4) timethres=60;
        if (rten==5) timethres=360;
        timethres*=24*60*60;
        for (ll i=0;i<asmt.size();i++) {
            ll distime=strtm2int(asmt[i].date);
            if (difftime(chrono::system_clock::to_time_t(chrono::system_clock::now()),distime)>timethres) {
                asmt.erase(asmt.begin()+i);
                i--;
            }
        }
    }
    bool asdisabled=false;
    if (impissue&&autosave) {
        cout<<"Autosave has been disabled."<<endl;
        autosave=0;
        asdisabled=true;
    }
    for (ll i=selaud.size()-1;i>=0;i--) {
        if (selaud[i]>=audio.size()) selaud.erase(selaud.begin()+i);
        i--;
    }
    ll grp=0;
    vector<vector<strWithInt>>showMore;
    
    bool compilerExists=false;
    bool compilerVirgin=true;
    vector<compilerInfo>compilers;
    if (SYS==APPL) {
        compilers.push_back((compilerInfo){"/usr/bin/g++","Xcode","%CODEPATH% -o %EXECPATH%"});
    } else {
        compilers.push_back((compilerInfo){"/Program Files (x86)/Dev-Cpp/MinGW64/bin/g++.exe","Dev-C++ (x86)","%CODEPATH% -o %EXECPATH%"});
        compilers.push_back((compilerInfo){"/Program Files/Dev-Cpp/MinGW64/bin/g++.exe","Dev-C++","%CODEPATH% -o %EXECPATH%"});
        compilers.push_back((compilerInfo){"/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe","Code::Blocks (x86)","%CODEPATH% -o %EXECPATH%"});
        compilers.push_back((compilerInfo){"/Program Files/CodeBlocks/MinGW/bin/g++.exe","Code::Blocks","%CODEPATH% -o %EXECPATH%"});
    }
    
    setupCompiler(newusrcompile,compilerExists,compilers,compilerVirgin);
    
    ll defaultCompilerID=-1;
    
    if (!prefersCompiler) for (ll i=0;i<compilers.size()&&defaultCompilerID==-1;i++) if (compilers[i].alias==preferredCompiler) defaultCompilerID=i;
    
    bool canLaunch=true;
//    for (ll i=0;i<db.size()&&canLaunch;i++) if (db[i]==' ') canLaunch=false;
//    canLaunch=canLaunch||(SYS==APPL);
//    if (!canLaunch) cout<<"Critical error! CodeAssign cannot launch helpers. This is due to a limitation in Windows. Make sure there is no space in your data path!"<<endl;
    //launch helpers
    if (compilerExists&&canLaunch) {
        cout<<"Launching helpers..."<<endl;
        for (ll i=1;i<=mltcore;i++) {
            auto testTmOut = chrono::high_resolution_clock::now();
            makeDir(db+"tmp/"+to_string(i)+"/");
            ofstream markExist(db+"tmp/"+to_string(i)+"/exists");
            markExist.close();
            //Copy over openhelper
            copyf(db+"openhelper"+exePost, db+"tmp2/openhelper"+to_string(i)+exePost, 0);
            //Open openhelper
            //Write to the bridge
            ofstream foldout(db+"tmp/pth.bridge");
            if (!foldout.good()) cout<<"Critical error! Error accessing path bridge in tmp."<<endl;
            foldout<<i;
            foldout.close();
            launchNewProcess(db+"tmp2/openhelper"+to_string(i)+exePost);
            while (true) {
                ifstream signaltest(db+"tmp/"+to_string(i)+"/firini.signal");
                if (signaltest.good()) {
                    signaltest.close();
                    break;
                }
                usleep(1e4);
                if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - testTmOut).count()>5000) {
                    cout<<"Error! Timeout for helper "<<i<<endl;
                    break;
                }
            }
        }
    }
//    cout<<"Alert! This is a testing version of CodeAssign."<<endl;
    while (true) {
        if (errorlog.size()) {
            cout<<"Errors logged during initialization:"<<endl;
            for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
            enterToCont();
            errorlog.clear();
        }
        ll chosenID=0;
        string target="";
        bool vq = false;
        if (impissue) {
            if (impfsVers) cout<<"!!!Contents file has wrong file system version. Press [F] to force quit. This will prevent corruption of the file"<<endl<<"You will lose all unsaved data. ";
            else cout<<"!!!Contents file corrupted. Press [F] to force quit. This will prevent further corruption of the file"<<endl<<"You will lose all unsaved data. ";
            if (asdisabled&&!autosave) {
                cout<<"Autosave has been disabled."<<endl;
            } else cout<<endl;
        }
        if (newUsr) {
            cout<<"Welcome to CodeAssign! CodeAssign is a code evaluator, allowing you to do programming questions and elevate your programming skills. Let's get started."<<endl;
        } else {
            cout<<"Welcome to CodeAssign Phoenix (1.5.7)"<<endl;
        }
        if (debugMd) cout<<"Alert! Debug mode enabled! Type \"debug\" to disable debug mode."<<endl;
        if (newUsr) {
            cout<<"Please select an action."<<endl<<"[1]New problem <- Open programming questions"<<endl<<"[2]Achievements <- Fun achievements for you to complete!"<<endl<<"[3]Settings <- Click in here to check out some extra options!"<<endl<<"[4]Quit"<<endl;
        } else {
            cout<<"Please select an action."<<endl<<"[1]New problem"<<endl<<"[2]Achievements"<<endl<<"[3]Settings"<<endl<<"[4]Quit"<<endl;
        }
        newUsr=false;
        if (rand()%4==0) cout<<"Tip:"<<tips[rand()%tips.size()]<<endl;
        cout<<"--------"<<endl;
        if (asmt.size()==0) {
            cout<<"Why don't you go get some problems to do?"<<endl;
        }
        sort(asmt.begin(),asmt.end(),cmp);
        bool containsNonK=false,containsK=false;
        for (ll i=0;i<asmt.size()&&(!containsNonK||!containsK);i++) {
            if (asmt[i].score<100) containsNonK=true;
            else containsK=true;
        }
            showMore.clear();
            vector<strWithInt>curGroupContents;
            ll curGrpCnt=0;
             if (containsNonK) {
                curGroupContents.push_back((strWithInt){"Incomplete Problems:",-1});
                for (ll i=0;i<asmt.size();i++) {
                    if (asmt[i].score<100) {
                        curGroupContents.push_back((strWithInt){asmt[i].name+":"+to_string(asmt[i].score),i});
                        curGrpCnt++;
                        if (ovrflw!=0) {
                            if (curGrpCnt%ovrflw==0) {
                                showMore.push_back(curGroupContents);
                                curGroupContents.clear();
                            }
                        }
                    }
                }
            }
            if (curGroupContents.size()!=0&&containsK) curGroupContents.push_back((strWithInt){"--------",-1});
            if (containsK) {
                curGroupContents.push_back((strWithInt){"Completed Problems:",-1});
                for (ll i=0;i<asmt.size();i++) {
                    if (asmt[i].score==100) {
                        curGroupContents.push_back((strWithInt){asmt[i].name+":"+to_string(asmt[i].score),i});
                        curGrpCnt++;
                        if (ovrflw!=0) {
                            if (curGrpCnt%ovrflw==0) {
                                showMore.push_back(curGroupContents);
                                curGroupContents.clear();
                            }
                        }
                    }
                }
            }
            if (curGroupContents.size()>0) {
                showMore.push_back(curGroupContents);
            }
        ll outcnt = 5;
        vector<ll>hwpoint;
        if (showMore.size()<=grp) grp=showMore.size()-1;
        if (asmt.size()) {
            for (ll i=0;i<showMore[grp].size();i++) {
                if (showMore[grp][i].mtd!=-1) {
                    cout<<"["<<outcnt<<"]";
                    hwpoint.push_back(showMore[grp][i].mtd);
                    outcnt++;
                }
                cout<<showMore[grp][i].ply<<endl;
            }
        }
        ll extended=outcnt;
        ll prevID=-1,nxtID=-1,jmpID=-1;
        if (showMore.size()>1) {
            if (showMore.size()-1) {
                cout<<"--------"<<endl<<"Page "<<grp+1<<" of "<<showMore.size()<<endl<<"["<<extended++<<"]Jump to page"<<endl;
                jmpID=extended-1;
            }
            if (grp!=0) {
                cout<<"["<<extended++<<"]Go to previous page"<<endl;
                prevID=extended-1;
            }
            if (grp!=showMore.size()-1) {
                cout<<"["<<extended++<<"]Go to next page"<<endl;
                nxtID=extended-1;
            }
        }
        string homeans;
        getline(cin,homeans);
        ll homeansNum=atoll(homeans.c_str());
        if (homeans!="4") clc();
        if (homeans=="1") {
            cout<<"Please drag the problem file into this window. (.hwfx)"<<endl;
            string pkgpth;
            getline(cin,pkgpth);
            nml(pkgpth);
            ifstream pkgverf(pkgpth);
            if (pkgverf.good()) {
                pkgverf.close();
                grp=0;
                if (pkgpth.size()>=5) {
                    if (pkgpth.substr(pkgpth.size()-5,5)!=".hwfx") {
                        cout<<"Unidentified file. Do not tamper with file extensions!"<<endl;
                    }
                } else cout<<"Unidentified file. Do not tamper with file extensions!"<<endl;
                if (!readHwfx(pkgpth,vq,target,chosenID,debugMd,aesKey,randname,db,asmt,errorlog)) cout<<"Error importing file!"<<endl;
                if (errorlog.size()) {
                    cout<<"File corrupted. Logged errors during file import:"<<endl;
                    for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                    vq=false;
                    enterToCont();
                    errorlog.clear();
                    clc();
                }
                if (autosave) savedata();
            } else {
                cout<<"Error reading package."<<endl;
            }
        } else if (homeans=="2") {
            cout<<"Achievements"<<endl<<"--------"<<endl<<"Academic:"<<endl<<"Success!:";
            if (cor>=1) cout<<"Completed "<<cor<<"/1 correct tries"<<endl;
            else cout<<"Complete "<<cor<<"/1 correct tries"<<endl;
            cout<<"More!:";
            if (cor>=10) cout<<"Completed "<<cor<<"/10 correct tries"<<endl;
            else cout<<"Complete "<<cor<<"/10 correct tries"<<endl;
            cout<<"Unstoppable!:";
            if (cor>=100) cout<<"Completed "<<cor<<"/100 correct tries"<<endl;
            else cout<<"Complete "<<cor<<"/100 correct tries"<<endl;
            cout<<"Godlike!:";
            if (cor>=1000) cout<<"Completed "<<cor<<"/1000 correct tries"<<endl;
            else cout<<"Complete "<<cor<<"/1000 correct tries"<<endl;
            cout<<"You can try again:";
            if (incor>=1) cout<<incor<<"/1 incorrect tries"<<endl;
            else cout<<incor<<"/1 incorrect tries done"<<endl;
            cout<<"Get up!:";
            if (incor>=10) cout<<incor<<"/10 incorrect tries"<<endl;
            else cout<<incor<<"/10 incorrect tries done"<<endl;
            cout<<"Keep going!:";
            if (incor>=100) cout<<incor<<"/100 incorrect tries"<<endl;
            else cout<<incor<<"/100 incorrect tries done"<<endl;
            cout<<"Nearly there!:";
            if (incor>=1000) cout<<incor<<"/1000 incorrect tries"<<endl;
            else cout<<incor<<"/1000 incorrect tries done"<<endl;
            cout<<"--------"<<endl<<"Secrets:"<<endl;
            for (ll i=0;i<easter.size();i++) {
                cout<<easter[i].ach<<":";
                if (secret[i]) cout<<"Input \""<<easter[i].trig<<"\" into the main menu."<<endl;
                else cout<<"A secret is required"<<endl;
            }
            cout<<"--------"<<endl;
            enterToCont();
            clc();
        } else if (homeans=="3") {
            while (true) {
                //TODO:Add compiler options
                cout<<"Settings"<<endl<<endl<<"GENERAL"<<endl<<"--------"<<endl<<"[1]Sort:";
                if (srtmode==1) cout<<"Alphabetically, increasing";
                else if (srtmode==2) cout<<"Alphabetically, decreasing";
                else if (srtmode==3) cout<<"Scores, increasing";
                else if (srtmode==4) cout<<"Scores, decreasing";
                else if (srtmode==5) cout<<"Date, increasing";
                else if (srtmode==6) cout<<"Date, decreasing";
                else cout<<"Error";
                if (settingshelp) cout<<" <- Change how problems are sorted in the main menu";
                cout<<endl;
                cout<<"[2]Keep problems for:";
                if (rten==1) cout<<"1 week";
                else if (rten==2) cout<<"2 weeks";
                else if (rten==3) cout<<"1 month";
                else if (rten==4) cout<<"2 months";
                else if (rten==5) cout<<"6 months";
                else if (rten==6) cout<<"Forever";
                else cout<<"Error";
                if (settingshelp) cout<<" <- Delete problems after a set amount of time";
                cout<<endl;
                cout<<"[3]Threads:";
                if (mltcore>1) cout<<mltcore<<" threads";
                else if (mltcore) cout<<mltcore<<" thread";
                else cout<<"Error";
                if (settingshelp) cout<<" <- Try this to make CodeAssign submissions faster!";
                cout<<endl;
                cout<<"[4]Sound:";
                if (sound) {
                    if (selaud.size()==0) cout<<"No song selected";
                    else if (audio.size()==selaud.size()) cout<<"All";
                    else {
                        cout<<audio[selaud[0]].sname;
                        if (selaud.size()==2) cout<<" and "<<selaud.size()-1<<" other...";
                        else if (selaud.size()>=2) cout<<" and "<<selaud.size()-1<<" others...";
                    }
                }
                else cout<<"Disabled";
                if (settingshelp) cout<<" <- Play a song on successfully completing a question.";
                cout<<endl;
                cout<<"[5]Autosave:";
                if (autosave) cout<<"Enabled";
                else {
                    if (asdisabled) cout<<"Disabled due to import issue";
                    else cout<<"Disabled";
                }
                if (settingshelp) cout<<" <- Automatically saves new progress and changes. Disable this if there's lag when closing menus.";
                cout<<endl;
                cout<<"[6]Limit items on one page to:";
                if (ovrflw) cout<<ovrflw<<" items";
                else cout<<"Infinite";
                if (settingshelp) cout<<" <- Too many items on the main menu? Use this to split them into pages!";
                cout<<endl;
//                cout<<"[7]Language:";
//                if (evalLang==0) cout<<"C++";
//                else if (evalLang==1) cout<<"Python";
//                else if (evalLang==2) cout<<"Java";
//                else cout<<"Error";
//                if (settingshelp) cout<<" <- Change your CodeAssign language";
//                cout<<endl;
                
                cout<<endl;
                if (settingshelp) cout<<"ADVANCED <- Don't touch these unless you know what you're doing";
                else cout<<"ADVANCED";
                cout<<endl<<"--------"<<endl;
                cout<<"[7]Answer file:";
                if (anslock) cout<<lockedf<<endl;
                else cout<<"Disabled"<<endl;
                cout<<"[8]Compiler:";
                if (!compilerExists) cout<<"None"<<endl;
                else {
                    if (prefersCompiler) {
                        string rdcpkg=preferredCompiler;
                        while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                        cout<<"Custom("<<rdcpkg<<")"<<endl;
                    } else {
                        cout<<"CodeAssign "<<preferredCompiler<<endl;
                    }
                }
                cout<<"[9]Unicode Make:";
                if (unicodehandl==0) cout<<"Replace and warn"<<endl;
                else if (unicodehandl==1) cout<<"Ignore"<<endl;
                else cout<<"Replace"<<endl;
                cout<<"--------"<<endl;
                cout<<endl<<"[10]Exit Settings"<<endl;
                string setans;
                settingshelp=0;
                getline(cin,setans);
                clc();
                if (setans=="1") {
                    cout<<"Select sort mode:"<<endl<<"[1]Alphabetically, increasing"<<endl<<"[2]Alphabetically, decreasing"<<endl<<"[3]Scores, increasing"<<endl<<"[4]Scores, decreasing"<<endl<<"[5]Date, increasing"<<endl<<"[6]Date, decreasing"<<endl;
                    string srtans;
                    getline(cin,srtans);
                    if (atoll(srtans.c_str())>=1&&atoll(srtans.c_str())<=6) {
                        srtmode=atoll(srtans.c_str());
                    }
                    clc();
                    if (autosave) savedata();
                } else if (setans=="2") {
                    cout<<"About keep problems:Problems older than the set time will be automatically deleted."<<endl<<"Select your preferred time."<<endl<<"[1]1 week"<<endl<<"[2]2 weeks"<<endl<<"[3]1 month"<<endl<<"[4]2 months"<<endl<<"[5]6 months"<<endl<<"[6]Forever"<<endl;
                    string settmp;
                    getline(cin,settmp);
                    clc();
                    if (to_string(atoll(settmp.c_str()))==settmp) {
                        ll timetmp = atoll(settmp.c_str());
                        if (timetmp>0&&timetmp<7) {
                            rten=timetmp;
                            if (rten==1) cout<<"Problem time successfully set to 1 week."<<endl;
                            else if (rten==2) cout<<"Problem time successfully set to 2 weeks."<<endl;
                            else if (rten==3) cout<<"Problem time successfully set to 1 month."<<endl;
                            else if (rten==4) cout<<"Problem time successfully set to 2 months."<<endl;
                            else if (rten==5) cout<<"Problem time successfully set to 6 months."<<endl;
                            else if (rten==6) cout<<"Problem time successfully set to forever."<<endl;
                        }
                    }
                    if (autosave) savedata();
                } else if (setans=="3") {
                    cout<<"About multithreaded evaluation:Your processor may be capable of running multiple tasks at once. Using multithreading settings will evaluate data using more threads. "<<endl<<"Input the amount of threads."<<endl;
                    if (compcore==0) cout<<"Suggestion is unavailble."<<endl;
                    else if (compcore<=4) cout<<"Using one thread is recommended."<<endl;
                    else cout<<"Using "<<compcore/2-1<<" threads is recommended."<<endl;
                    string settmp;
                    getline(cin,settmp);
                    clc();
                    if (to_string(atoll(settmp.c_str()))==settmp) {
                        ll coretmp = atoll(settmp.c_str());
                        if (coretmp>0) {
                            if (coretmp<=10) {
                                if (coretmp<mltcore) {
                                    if (compilerExists&&canLaunch) {
                                        for (ll i=coretmp+1;i<=mltcore;i++) {
                                            ofstream stopsig(db+"tmp/"+to_string(i)+"/pth.bridge");
                                            if (!stopsig.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<" while planting STOP signal"<<endl;
                                            stopsig<<"STOP";
                                            stopsig.close();
                                        }
                                    }
                                } else if (coretmp>mltcore) {
                                    if (compilerExists&&canLaunch) {
                                        cout<<"Launching extra helpers..."<<endl;
                                        for (ll i=mltcore+1;i<=coretmp;i++) {
                                            makeDir(db+"tmp/"+to_string(i)+"/");
                                            ofstream markExist(db+"tmp/"+to_string(i)+"/exists");
                                            markExist.close();
                                            //Copy over openhelper
                                            copyf(db+"openhelper"+exePost,db+"tmp2/openhelper"+to_string(i)+exePost,0);
                                            //Open openhelper
                                            //Write to the bridge
                                            ofstream foldout(db+"tmp/pth.bridge");
                                            if (!foldout.good()) cout<<"Critical error! Error accessing path bridge in tmp."<<endl;
                                            foldout<<i;
                                            foldout.close();
                                            launchNewProcess(db+"tmp2/openhelper"+to_string(i)+exePost);
                                            while (true) {
                                                ifstream signaltest(db+"tmp/"+to_string(i)+"/firini.signal");
                                                if (signaltest.good()) {
                                                    signaltest.close();
                                                    break;
                                                }
                                                usleep(1e4);
                                            }
                                        }
                                    }
                                }
                                mltcore=coretmp;
                                if (mltcore==1) cout<<"Multithreading successfully set to 1 thread."<<endl;
                                else cout<<"Multithreading successfully set to "<<mltcore<<" threads."<<endl;
                            } else cout<<"Error! It is not advised to use more than 10 threads."<<endl;
                        } else cout<<"Error! A positive amount of threads must be used."<<endl;
                    }
                    if (autosave) savedata();
                } else if (setans=="4") {
                    if (SYS==APPL) {
                        bool dirlib=false;
                        bool soundlib=false;
                        ifstream libftest("/usr/local/lib");
                        if (libftest.good()) {
                            dirlib=true;
                        }
                        libftest.close();
                        if (dirlib) {
                            libftest.open("/usr/local/lib/libirrklang.dylib");
                            if (libftest.good()) {
                                soundlib=true;
                            }
                        }
                        if (dirlib&&soundlib) {
                            while (true) {
                                if (sound) {
                                    while (true) {
                                        cout<<"Current selectiion:";
                                        if (selaud.size()==audio.size()) cout<<"All";
                                        else if (selaud.size()==0) {
                                            cout<<"None";
                                        } else {
                                            cout<<audio[selaud[0]].sname;
                                            for (ll i=1;i<selaud.size();i++) {
                                                cout<<", "<<audio[selaud[i]].sname;
                                            }
                                        }
                                        bool soundcapped=true;
                                        if (audio.size()<35) soundcapped=false;
                                        cout<<endl<<"[1]Disable sound"<<endl;
                                        if (!soundcapped) cout<<"[2]Add new sounds"<<endl;
                                        cout<<"["<<3-soundcapped<<"]Delete sounds"<<endl<<"--------"<<endl<<"["<<4-soundcapped<<"]Select all"<<endl;
                                        map<char, ll>schoice;
                                        for (ll i=0;i<audio.size();i++) {
                                            if (i+5-soundcapped<=9) {
                                                cout<<"["<<i+5-soundcapped<<"]"<<audio[i].sname<<endl;
                                                schoice[i+5-soundcapped+'0']=i+1;
                                            } else {
                                                schoice['A'+i-5-soundcapped]=i+1;
                                                cout<<"["<<(char)('A'+i-5-soundcapped)<<"]"<<audio[i].sname<<endl;
                                            }
                                        }
                                        cout<<"Select songs to play after a correct submission..."<<endl;
                                        string songchoice;
                                        getline(cin,songchoice);
                                        bool fc[audio.size()];
                                        bool choosesounds=true;
                                        for (ll i=0;i<audio.size();i++) fc[i]=0;
                                        for (ll i=0;i<songchoice.length();i++) {
                                            if (songchoice[i]==4-soundcapped+'0') for (ll j=0;j<audio.size();j++) fc[j]=1;
                                            else if (schoice[songchoice[i]]!=0) fc[schoice[songchoice[i]]-1]=1;
                                            else if (songchoice[i]=='2') {
                                                choosesounds=false;
                                                cout<<"Alert! Song must be in WAV format."<<endl<<"Please drag the song into this window."<<endl;
                                                string newsoundpth;
                                                getline(cin,newsoundpth);
                                                nml(newsoundpth);
                                                ifstream newsoundtest(newsoundpth);
                                                if (newsoundtest.good()) {
                                                    newsoundtest.close();
                                                    cout<<"What is the name of the song?"<<endl;
                                                    string songn;
                                                    getline(cin,songn);
                                                    string songf="";
                                                    while (true) {
                                                        songf="";
                                                        for (ll j=0;j<16;i++) songf+=randname[rand()%36];
                                                        ifstream testDup(db+songf+".wav");
                                                        if (!testDup.good()) {
                                                            testDup.close();
                                                            break;
                                                        }
                                                        testDup.close();
                                                    }
                                                    fcopy(newsoundpth,db+songf+".wav");
                                                    audio.push_back({songf,songn});
                                                    cout<<"Song "<<songn<<" imported successfully."<<endl;
                                                } else cout<<"Error reading song!"<<endl;
                                                //add new sounds
                                                break;
                                            } else if (songchoice[i]==3-soundcapped+'0') {
                                                choosesounds=false;
                                                for (ll i=0;i<audio.size();i++) {
                                                    cout<<"[";
                                                    i<9?cout<<i+1:cout<<(char)('A'+i-9);
                                                    cout<<"]"<<audio[i].sname<<endl;
                                                }
                                                cout<<"What song do you want to delete?"<<endl;
                                                string delschoice;
                                                getline(cin,delschoice);
                                                set<ll>delchoice;
                                                for (ll i=0;i<delschoice.length();i++) delchoice.insert(delschoice[i]>='A'?delschoice[i]-'A'+10-1:delschoice[i]-'0'-1);
                                                vector<ll>todel;
                                                for (set<ll>::iterator i=delchoice.begin();i!=delchoice.end();i++) if (*i<audio.size()) todel.push_back(*i); else break;
                                                if (todel.size()) cout<<"Are you sure you want to delete ";
                                                for (ll i=0;i<todel.size()-1;i++) cout<<audio[todel[i]].sname<<", ";
                                                if (todel.size()>1) cout<<"and ";
                                                cout<<audio[todel[todel.size()-1]].sname<<"? This action is irreversible. Input CONFIRM to confirm."<<endl;
                                                string conf;
                                                getline(cin,conf);
                                                if (conf=="CONFIRM") {
                                                    for (ll i=todel.size()-1;i>=0;i--) audio.erase(audio.begin()+todel[i]);
                                                    sort(selaud.begin(),selaud.end());
                                                    for (ll i=selaud.size();i>=0;i++) {
                                                        if (selaud[i]>=audio.size()) selaud.erase(selaud.begin()+i);
                                                        i--;
                                                    }
                                                }
                                            } else if (songchoice=="1") {
                                                cout<<"Sound disabled"<<endl;
                                                sound=false;
                                                break;
                                            }
                                        }
                                        if (choosesounds) {
                                            selaud.clear();
                                            for (ll i=0;i<audio.size();i++) if (fc[i]) selaud.push_back(i);
                                            break;
                                        }
                                    }
                                    break;
                                } else {
                                    cout<<"Sound enabled"<<endl;
                                    sound=true;
                                }
                            }
                        } else {
                            cout<<"Using sound requires the installation of external libraries. This will require an administrator password. Enter y to proceed."<<endl;
                            string confins;
                            getline(cin,confins);
                            if (confins=="y"||confins=="Y") {
                                ofstream sil(db+"tmp/sil.txt");
                                if (!sil.good()) cout<<"Error writing silent file to tmp(SOUND)"<<endl;
                                sil<<"SILENCE";
                                sil.close();
                                removeWithinFolder(db+"tmp/");
                                launchNewProcess(db+"soundsetup"+exePost);
                                ifstream donetest;
                                while (true) {
                                    donetest.open(db+"tmp/installed.txt");
                                    if (donetest.good()) break;
                                    donetest.close();
                                    usleep(1e4);
                                }
                                donetest.close();
                                ifstream instest("/usr/local/lib/libirrklang.dylib");
                                if (instest.good()) {
                                    cout<<"Sound enabled"<<endl;
                                    sound=true;
                                } else cout<<"Install failed."<<endl;
                                instest.close();
                            }
                        }
                        clc();
                        if (autosave) savedata();
                    } else {
                        cout<<"The Windows version of CodeAssign does not support sound yet."<<endl;
                    }
                } else if (setans=="5") {
                    if (autosave) {
                        cout<<"Autosave disabled."<<endl;
                        autosave=false;
                    } else {
                        if (asdisabled) {
                            cout<<"Are you sure about enabling autosave? You may corrupt files. Input CONFIRM to confirm."<<endl;
                            string conf;
                            getline(cin,conf);
                            clc();
                            if (conf=="CONFIRM") {
                                cout<<"Autosave enabled."<<endl;
                                autosave=true;
                                savedata();
                                asdisabled=false;
                            }
                        } else {
                            cout<<"Autosave enabled."<<endl;
                            autosave=true;
                            savedata();
                        }
                    }
                } else if (setans=="6") {
                    cout<<"How many items to limit to? Input 0 for no limits."<<endl;
                    while (true) {
                        string itmlim;
                        getline(cin,itmlim);
                        if (to_string(atoll(itmlim.c_str()))!=itmlim) {
                            cout<<"Invalid input!"<<endl;
                            continue;
                        }
                        if (atoll(itmlim.c_str())>=0) {
                            ovrflw=atoll(itmlim.c_str());
                            break;
                        } else {
                            cout<<"Invalid input!"<<endl;
                        }
                    }
                    clc();
                } else if (setans=="dev") {
//                    if (true) {
//                        cout<<"In development!"<<endl;
//                    } else {
//                        cout<<"Select language:"<<endl;
//                        cout<<"[1]C++";
//                        if (!compilerExists) cout<<" Compiler not found";
//                        cout<<endl;
//                    }
                } else if (setans=="7") {
                    cout<<"About answer file:Answer file sets a specific .cpp file for submissions."<<endl;
                    string anslockans;
                    if (anslock) {
                        while (true) {
                            cout<<"Current answer file:"<<lockedf<<endl<<"[1]Disable answer file"<<endl<<"[2]Change answer file"<<endl<<"[3]Exit"<<endl;
                            getline(cin,anslockans);
                            if (anslockans=="1") {
                                anslock=0;
                                clc();
                                cout<<"Answer file disabled"<<endl;
                                break;
                            } else if (anslockans=="2") {
                                cout<<"Drag the answer file into this window."<<endl;
                                getline(cin,anslockans);
                                nml(anslockans);
                                if (anslockans.find("dwtReserved::")==string::npos) {
                                    cout<<"Answer file set to "<<anslockans<<endl;
                                    clc();
                                    lockedf=anslockans;
                                    break;
                                } else cout<<"Filename cannot contain \"dwtReserved::\"!"<<endl;
                            } else if (anslockans=="3") {
                                clc();
                                break;
                            }
                        }
                    } else {
                        while (true) {
                            cout<<"[1]Enable answer file"<<endl<<"[2]Exit"<<endl;
                            getline(cin,anslockans);
                            if (anslockans=="1") {
                                cout<<"Enter the path of the desired answer file."<<endl;
                                getline(cin,anslockans);
                                nml(anslockans);
                                if (anslockans.find("dwtReserved::")==string::npos) {
                                    lockedf=anslockans;
                                    anslock=1;
                                    break;
                                } else cout<<"Filename cannot contain \"dwtReserved::\"!"<<endl;
                            } else if (anslockans=="2") {
                                clc();
                                break;
                            }
                        }
                    }
                } else if (setans=="8") {
                    if (!compilerExists) {
                        while (true) {
                            cout<<"Current compiler:None"<<endl<<"[1]Enable compiling"<<endl<<"[2]Exit"<<endl;
                            string compileransa;
                            getline(cin,compileransa);
                            if (compileransa=="2") {
                                clc();
                                break;
                            } else if (compileransa=="1") {
                                clc();
                                compilerVirgin=true;
                                setupCompiler(newusrcompile,compilerExists,compilers,compilerVirgin);
                                break;
                            } else {
                                clc();
                                cout<<"Invalid response!"<<endl;
                            }
                        }
                    } else {
                        if (!prefersCompiler) {
                            while (true) {
                                cout<<"Current compiler:";
                                if (prefersCompiler) {
                                    string rdcpkg=preferredCompiler;
                                    while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                    cout<<"Custom("<<rdcpkg<<")"<<endl;
                                } else {
                                    cout<<"CodeAssign "<<preferredCompiler<<endl;
                                }
                                cout<<"It is not recommended to change these settings."<<endl<<"[1]Switch to a custom compiler"<<endl<<"[2]Exit"<<endl;
                                string compilergeta;
                                getline(cin,compilergeta);
                                if (compilergeta=="1") {
                                    clc();
                                    while (true) {
                                        cout<<"Drag the custom compiler into this window. Input exit to cancel."<<endl;
                                        string cusCompPath;
                                        getline(cin,cusCompPath);
                                        nml(cusCompPath);
                                        string compexit=cusCompPath;
                                        transform(compexit.begin(),compexit.end(),compexit.begin(),::tolower);
                                        if (compexit=="exit") break;
                                        ifstream testThere(cusCompPath);
                                        if (testThere.good()) {
                                            clc();
                                            prefersCompiler=true;
                                            preferredCompiler=cusCompPath;
                                            while (true) {
                                                cout<<"Current compiler format:"<<compilerFormat<<endl<<"Is this correct?"<<endl<<"[1]Yes"<<endl<<"[2]No"<<endl;
                                                string formatcor;
                                                getline(cin,formatcor);
                                                if (formatcor=="1") {
                                                    clc();
                                                    break;
                                                } else if (formatcor=="2") {
                                                    clc();
                                                    bool masterBreak=false;
                                                    while (true) {
                                                        cout<<"Please input the new format. Input exit to cancel."<<endl;
                                                        string tmpcompilerFormat;
                                                        getline(cin,tmpcompilerFormat);
                                                        string exitTry=tmpcompilerFormat;
                                                        transform(exitTry.begin(),exitTry.end(),exitTry.begin(),::tolower);
                                                        if (exitTry=="exit"){
                                                            clc();
                                                            masterBreak=true;
                                                            break;
                                                        }
                                                        if (tmpcompilerFormat.find("%CODEPATH%")!=string::npos&&tmpcompilerFormat.find("%EXECPATH")!=string::npos) {
                                                            compilerFormat=tmpcompilerFormat;
                                                            clc();
                                                            masterBreak=true;
                                                            break;
                                                        } else {
                                                            clc();
                                                            cout<<"Error! Compiler format must include %CODEPATH% (The path to the code) and %EXECPATH% (The path to the output executable)!"<<endl;
                                                        }
                                                    }
                                                    if (masterBreak) break;
                                                }
                                                clc();
                                            }
                                            break;
                                        } else {
                                            clc();
                                            cout<<"An error occured while finding the custom compiler."<<endl;
                                        }
                                    }
                                    break;
                                } else if (compilergeta=="2") {
                                    clc();
                                    break;
                                }
                            }
                        } else {
                            while (true) {
                                cout<<"Current compiler:";
                                if (prefersCompiler) {
                                    string rdcpkg=preferredCompiler;
                                    while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                    cout<<"Custom("<<rdcpkg<<")"<<endl;
                                } else {
                                    cout<<"CodeAssign "<<preferredCompiler<<endl;
                                }
                                ifstream testIfCustom;
                                ll goodpfcomp=-1;
                                for (ll i=0;i<compilers.size();i++) {
                                    testIfCustom.open(compilers[i].pth);
                                    if (testIfCustom.good()) {
                                        goodpfcomp=i;
                                        testIfCustom.close();
                                        break;
                                    }
                                    testIfCustom.close();
                                }
                                if (goodpfcomp==-1) {
                                    cout<<"No CodeAssign compiler can be found"<<endl;
                                }
                                cout<<"[1]Switch to another custom compiler"<<endl<<"[2]Change compiler format("<<compilerFormat<<")"<<endl;
                                if (goodpfcomp!=-1) {
                                    cout<<"[3]Switch to a CodeAssign compiler"<<endl<<"[4]Exit"<<endl;
                                } else {
                                    cout<<"[3]Exit"<<endl;
                                }
                                string compilerin;
                                getline(cin,compilerin);
                                if (compilerin=="1") {
                                    while (true) {
                                        clc();
                                        cout<<"Drag the custom compiler into this window. Input exit to cancel."<<endl;
                                        string cusCompPath;
                                        getline(cin,cusCompPath);
                                        nml(cusCompPath);
                                        string compexit=cusCompPath;
                                        transform(compexit.begin(),compexit.end(),compexit.begin(),::tolower);
                                        if (compexit=="exit") break;
                                        ifstream testThere(cusCompPath);
                                        if (testThere.good()) {
                                            clc();
                                            prefersCompiler=true;
                                            preferredCompiler=cusCompPath;
                                            while (true) {
                                                cout<<"Current compiler format:"<<compilerFormat<<endl<<"Is this correct?"<<endl<<"[1]Yes"<<endl<<"[2]No"<<endl;
                                                string formatcor;
                                                getline(cin,formatcor);
                                                if (formatcor=="1") {
                                                    clc();
                                                    break;
                                                } else if (formatcor=="2") {
                                                    clc();
                                                    bool masterBreak=false;
                                                    while (true) {
                                                        cout<<"Please input the new format. Input exit to cancel."<<endl;
                                                        string tmpcompilerFormat;
                                                        getline(cin,tmpcompilerFormat);
                                                        string exitTry=tmpcompilerFormat;
                                                        transform(exitTry.begin(),exitTry.end(),exitTry.begin(),::tolower);
                                                        if (exitTry=="exit"){
                                                            clc();
                                                            masterBreak=true;
                                                            break;
                                                        }
                                                        if (tmpcompilerFormat.find("%CODEPATH%")!=string::npos&&tmpcompilerFormat.find("%EXECPATH")!=string::npos) {
                                                            compilerFormat=tmpcompilerFormat;
                                                            clc();
                                                            masterBreak=true;
                                                            break;
                                                        } else {
                                                            clc();
                                                            cout<<"Error! Compiler format must include %CODEPATH% (The path to the code) and %EXECPATH% (The path to the output executable)!"<<endl;
                                                        }
                                                    }
                                                    if (masterBreak) break;
                                                }
                                                clc();
                                            }
                                        } else {
                                            clc();
                                            cout<<"An error occured while finding the custom compiler."<<endl;
                                        }
                                        break;
                                    }
                                } else if (compilerin=="2") {
                                    clc();
                                    while (true) {
                                        cout<<"Please input the new format. Input exit to cancel."<<endl;
                                        string tmpcompilerFormat;
                                        getline(cin,tmpcompilerFormat);
                                        string exitTry=tmpcompilerFormat;
                                        transform(exitTry.begin(),exitTry.end(),exitTry.begin(),::tolower);
                                        if (exitTry=="exit"){
                                            clc();
                                            break;
                                        }
                                        if (tmpcompilerFormat.find("%CODEPATH%")!=string::npos&&tmpcompilerFormat.find("%EXECPATH")!=string::npos) {
                                            compilerFormat=tmpcompilerFormat;
                                            clc();
                                            break;
                                        } else {
                                            clc();
                                            cout<<"Error! Compiler format must include %CODEPATH% (The path to the code) and %EXECPATH% (The path to the output executable)!"<<endl;
                                        }
                                    }
                                    clc();
                                } else if (goodpfcomp==-1&&compilerin=="3") {
                                    clc();
                                    break;
                                } else if (goodpfcomp!=-1&&compilerin=="3") {
                                    prefersCompiler=0;
                                    preferredCompiler=compilers[goodpfcomp].alias;
                                    compilerFormat=compilers[goodpfcomp].compForm;
                                    clc();
                                    break;
                                } else if (goodpfcomp!=-1&&compilerin=="4") {
                                    clc();
                                    break;
                                }
                            }
                        }
                    }
                } else if (setans=="9") {
                    clc();
                    cout<<"Select the method of Unicode handling"<<endl;
                    cout<<"[1]Replace and warn";
                    if (unicodehandl==0) cout<<" (Selected)";
                    cout<<": Replaces all known Unicode values and reports errors on unknown characters. Leaves non identified values as is."<<endl;
                    cout<<"[2]Replace";
                    if (unicodehandl==2) cout<<" (Selected)";
                    cout<<": Replaces all known Unicode values. Leaves non identified values as is."<<endl;
                    cout<<"[3]Ignore";
                    if (unicodehandl==1) cout<<" (Selected)";
                } else if (setans=="10") {
                    if (autosave) savedata();
                    clc();
                    break;
                }
            }
        } else if (homeans=="4") {
            //Save
            savedata();
            if (compilerExists&&canLaunch) {
                for (ll i=1;i<=mltcore;i++) {
                    ifstream testEx(db+"tmp/"+to_string(i)+"/exists");
                    if (!testEx.good()) {
                        testEx.close();
                        makeDir(db+"tmp/"+to_string(i)+"/");
                    }
                }
                for (ll i=1;i<=mltcore;i++) {
                    ofstream stopsig(db+"tmp/"+to_string(i)+"/pth.bridge");
                    if (!stopsig.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<" while planting STOP signal"<<endl;
                    stopsig<<"STOP";
                    stopsig.close();
                }
            }
            delete[] secret;
            return 0;
        } else if (tolower(homeans[0])=='f'&&homeans.length()==1&&impissue) {
            if (compilerExists&&canLaunch) {
                for (ll i=1;i<=mltcore;i++) {
                    ofstream stopsig(db+"tmp/"+to_string(i)+"/pth.bridge");
                    if (!stopsig.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<" while planting STOP signal"<<endl;
                    stopsig<<"STOP";
                    stopsig.close();
                }
            }
            delete[] secret;
            return 0;
        } else if (to_string(homeansNum)==homeans&&homeansNum>=5&&homeansNum<outcnt) { //MARK:Set file
            chosenID=hwpoint[atoll(homeans.c_str())-5];
            if (chosenID==-1) cout<<"Invalid response."<<endl;
            else {
                vq=true;
                target = asmt[chosenID].id;
            }
        } else if (to_string(homeansNum)==homeans&&homeansNum==prevID&&prevID!=-1) grp--;
        else if (to_string(homeansNum)==homeans&&homeansNum==nxtID&&nxtID!=-1) grp++;
        else if (to_string(homeansNum)==homeans&&homeansNum==jmpID&&jmpID!=-1) {
            while (true) {
                cout<<"Which page would you like to jump to? Current page:Page "<<grp+1<<" of "<<showMore.size()<<endl;
                string jmpNum;
                getline(cin,jmpNum);
                ll jmpNumNum=atoll(jmpNum.c_str());
                if (to_string(jmpNumNum)==jmpNum&&jmpNumNum>=1&&jmpNumNum<=showMore.size()) {
                    grp=jmpNumNum-1;
                    break;
                } else cout<<"Jump number invalid!"<<endl;
            }
        } else {
            string vtmp=homeans;
            transform(homeans.begin(),homeans.end(),vtmp.begin(),::tolower);
            bool hitach = false;
            for (ll i=0;i<easter.size();i++) {
                if (homeans==easter[i].trig) {
                    hitach=true;
                    cout<<easter[i].msg<<endl;
                    if (!secret[i]) cout<<"Achievement \""<<easter[i].ach<<"\" complete."<<endl;
                    secret[i]=true;
                    break;
                }
            }
            if (hitach&&autosave) savedata();
            if (!hitach) {
                if (vtmp=="changelog") {
                    changelog();
                } else if (vtmp=="kill") {
                    for (ll i=1;i<=mltcore;i++) {
                        ofstream stopsig(db+"tmp/"+to_string(i)+"/pth.bridge");
                        if (!stopsig.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<" while planting STOP signal"<<endl;
                        stopsig<<"STOP";
                        stopsig.close();
                    }
                    delete[] secret;
                    return 0;
                } else if (vtmp=="batchmake") {
                    #if CANUSEFS
                    //Batch make
                    cout<<"Drag the directory to batch make from into this window."<<endl;
                    string dirBtch;
                    getline(cin,dirBtch);
                    chrono::steady_clock::time_point grs=chrono::steady_clock::now();
                    nml(dirBtch);
                    if (!fs::exists(dirBtch)) {
                        cout<<"Error! Directory does not exist!"<<endl;
                        continue;
                    }
                    if (!fs::is_directory(dirBtch)) {
                        cout<<"Error! Directory does not exist!"<<endl;
                        continue;
                    }
                    vector<string>chkobj;
                    if (true) {
                        chkobj.push_back("description.txt");
                        chkobj.push_back("in.txt");
                        chkobj.push_back("out.txt");
                        chkobj.push_back("title.txt");
                        chkobj.push_back("in1.txt");
                        chkobj.push_back("out1.txt");
                    }
                    bool gotton=false;
                    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dirBtch)) {
                        if (fs::is_directory(entry.path())) {
                            map<string,bool>gotF;
                            for (const fs::directory_entry& disf : fs::directory_iterator(entry.path())) {
                                string rdcpkg=disf.path();
                                while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                gotF[rdcpkg]=true;
                            }
                            bool isQ=true;
                            for (ll i=0;i<chkobj.size()&&isQ;i++) if (!gotF[chkobj[i]]) isQ=false;
                            if (isQ) {
                                gotton=true;
                                string pkgpth=entry.path();
                                string outpth=pkgpth+".hwfx";
                                ifstream testOverlap(outpth);
                                if (testOverlap.good()) {
                                    cout<<outpth<<" already exists."<<endl;
                                    continue;
                                }
                                string rdcpkg=pkgpth;
                                while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                
                                bool impFail;
                                string disTtl=rdcpkg;
                                string newPkg;
                                writeHwfx(pkgpth,impFail,disTtl,newPkg,errorlog);
                                if (errorlog.size()) {
                                    cout<<"Errors occured when batch making:"<<endl;
                                    for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                                    errorlog.clear();
                                    enterToCont();
                                }
                                if (impFail) cout<<"Processing of "<<disTtl<<" failed."<<endl;
                                else {
                                    if (writeCore(outpth,newPkg,aesKey,errorlog)) cout<<"Processing of "<<disTtl<<" failed."<<endl;
                                    else cout<<disTtl<<" successfully processed."<<endl;
                                    if (errorlog.size()) cout<<"Errors occured in batchmake writeCore:"<<endl;
                                    for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                                    if (errorlog.size()) enterToCont();
                                    errorlog.clear();
                                }
                            }
                        }
                    }
                    if (!gotton) cout<<"No valid directories found."<<endl;
                    if (debugMd) cout<<"Batch job done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
                    #else
                        cout<<"Error! Filesystem is unavailable!"<<endl;
                    #endif
                } else if (vtmp=="hreset") {
                    //delete all except for user homework files and .dwt (can only be done with fs)
                    
                    //reinstall all helpers
                    install(0);
                } else if (vtmp=="showdir") {
                    cout<<"CodeAssign Directory: "<<db<<endl;
                } else if (vtmp=="batchimport") {
                    #if CANUSEFS
                    //Batch import
                    cout<<"Drag the directory to batch import from into this window."<<endl;
                    string dirBtch;
                    getline(cin,dirBtch);
                    chrono::steady_clock::time_point grs=chrono::steady_clock::now();
                    nml(dirBtch);
                    ll latentDebug=debugMd;
                    if (debugMd) debugMd=0;
                    if (fs::exists(dirBtch)) {
                        if (fs::is_directory(dirBtch)) {
                            for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dirBtch)) {
                                if (fs::is_regular_file(entry.path())) {
                                    string chk=entry.path();
                                    if (chk.size()>=5&&chk.substr(chk.size()-5,5)==".hwfx") {
                                        bool dummy1=0;
                                        string dummy2="";
                                        ll dummy3=0;
                                        string rdcpth=chk;
                                        while (rdcpth.find("/")!=string::npos) rdcpth=rdcpth.substr(rdcpth.find("/")+1);
                                        if (readHwfx(chk,dummy1,dummy2,dummy3,debugMd,aesKey,randname,db,asmt,errorlog)) cout<<"Successfully imported "<<rdcpth<<endl;
                                        else cout<<"Error importing "<<rdcpth<<endl;
                                        if (errorlog.size()) {
                                            cout<<"Logged errors during batch import:"<<endl;
                                            for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                                            enterToCont();
                                            errorlog.clear();
                                        }
                                    }
                                }
                            }
                            if (autosave) savedata();
                        }
                    } else cout<<"Error reading directory!"<<endl;
                    debugMd=latentDebug;
                    if (debugMd) cout<<"Batch job done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
                    #else
                        cout<<"Error! Filesystem is unavailable!"<<endl;
                    #endif
                } else if (vtmp=="clearf") {
                    cout<<"Clear all program files? This action is irreversible(Input CLEAR in all caps to confirm)"<<endl;
                    string clearall;
                    getline(cin,clearall);
                    if (clearall=="CLEAR") {
                        removeFolder(db);
                        cout<<"Files cleared. Program will now force quit."<<endl;
                        delete[] secret;
                        return 0;
                    }
                } else if (vtmp=="id") {
                    cout<<"How many random length 16 IDs?"<<endl;
                    string idin;
                    getline(cin,idin);
                    if (atoll(idin.c_str())>0) {
                        for (ll i=0;i<atoll(idin.c_str());i++) {
                            string idout="";
                            for (ll j=0;j<16;j++) idout+=randname[rand()%36];
                            cout<<idout<<endl;
                        }
                    }
                } else if (vtmp=="const") cout<<"System:"<<SYS<<endl<<"Filesystem:"<<CANUSEFS<<endl;
                else if (vtmp=="debug") {
                    if (debugMd) {
                        cout<<"Debug mode disabled."<<endl;
                        debugMd=0;
                    } else {
                        cout<<"Debug mode enabled."<<endl;
                        debugMd=1;
                    }
                } else if (vtmp=="make") {
                    cout<<"hwfX package maker. Example hierarchy:"<<endl<<"Your folder"<<endl<<"  |- title.txt:Title"<<endl<<"  |- description.txt:Problem description"<<endl<<"  |- exin1.txt:Example input 1"<<endl<<"  |- exin2.txt:Example input 2"<<endl<<"  |- exout1.txt:Example output 1 to match up with exin1"<<endl<<"  |- exout2.txt:Example output 2"<<endl<<"  |- in.txt:Input requirements"<<endl<<"  |- out.txt:Output requirements"<<endl<<"  |- in1.txt:Testing input set #1"<<endl<<"  |- in2.txt:Testing input set #2"<<endl<<"  |- out1.txt:Testing output #1, matches up with in1.txt"<<endl<<"  |- out2.txt:Testing output #2"<<endl<<"  |-------"<<endl<<"You may include any amount of test sets. 10-20 is recommended."<<endl<<"You may also include any amount of example inputs. 1-3 is recommended."<<endl<<"Drag the problem directory into this window."<<endl;
                    string pkgpth;
                    getline(cin,pkgpth);
                    nml(pkgpth);
                    string outpth=pkgpth+".hwfx";
                    bool gogogo=false;
                    while (true) {
                        ifstream testOverlap(outpth);
                        bool goOn=false;
                        if (testOverlap.good()) {
                            while (true) {
                                clc();
                                cout<<"The file "<<outpth<<" already exists. Do you want to "<<endl<<"[1]Overwrite"<<endl<<"[2]Choose another path"<<endl<<"[3]Stop"<<endl;
                                string ovrlpdec;
                                getline(cin,ovrlpdec);
                                if (ovrlpdec=="1") {
                                    goOn=true;
                                    break;
                                } else if (ovrlpdec=="2") {
                                    string rdcpkg=pkgpth;
                                    while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                    rdcpkg+=".hwfx";
                                    cout<<"Please drag the directory for the file "<<rdcpkg<<" into this window."<<endl;
                                    getline(cin,outpth);
                                    nml(outpth);
                                    outpth=outpth+"/"+rdcpkg;
                                    testOverlap.close();
                                    break;
                                } else if (ovrlpdec=="3") {
                                    goOn=true;
                                    gogogo=true;
                                    break;
                                } else cout<<"Invalid response."<<endl;
                            }
                        } else {
                            ofstream outTst(outpth);
                            if (outTst.good()) break;
                            else cout<<"Failed to write to "<<outpth<<". Do you want to "<<endl<<"[1]Change output file"<<endl<<"[2]Stop"<<endl;
                            string badWrtRes;
                            getline(cin,badWrtRes);
                            while (true) {
                                if (badWrtRes=="1") {
                                    string rdcpkg=pkgpth;
                                    while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                                    rdcpkg+=".hwfx";
                                    cout<<"Please drag the directory for the file "<<rdcpkg<<" into this window."<<endl;
                                    getline(cin,outpth);
                                    nml(outpth);
                                    outpth=outpth+"/"+rdcpkg;
                                    break;
                                } else {
                                    gogogo=goOn=true;
                                    break;
                                }
                            }
                        }
                        if (goOn) break;
                    }
                    string rdcpkg=pkgpth;
                    while (rdcpkg.find("/")!=string::npos) rdcpkg=rdcpkg.substr(rdcpkg.find("/")+1);
                    if (gogogo) continue;
                    string masterPkg;
                    ll contCnt=0;
                    while (true) {
                        bool impFail;
                        string disTtl=rdcpkg;
                        string newPkg;
                        writeHwfx(pkgpth,impFail,disTtl,newPkg,errorlog);
                        clc();
                        if (impFail) cout<<"Importing of "<<disTtl<<" failed."<<endl;
                        else {
                            cout<<disTtl<<" successfully processed."<<endl;
                            masterPkg+=newPkg;
                            contCnt++;
                        }
                        if (errorlog.size()) {
                            cout<<"Logged errors during make:"<<endl;
                            for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                            enterToCont();
                            errorlog.clear();
                        }
                        cout<<"Would you like to add another file to the package?"<<endl<<"[1]Yes"<<endl<<"[2]No"<<endl;
                        string doItAgain;
                        while (true) {
                            getline(cin,doItAgain);
                            if (doItAgain!="1"&&doItAgain!="2") cout<<"Invalid response."<<endl;
                            else break;
                        }
                        clc();
                        if (doItAgain=="1") {
                            cout<<"Please drag the problem directory into this window."<<endl;
                            getline(cin,pkgpth);
                            nml(pkgpth);
                        } else break;
                    }
                    if (contCnt>0) {
                        if (!writeCore(outpth,masterPkg,aesKey,errorlog)) cout<<"hwfX file created at "<<outpth<<endl;
                        if (errorlog.size()) {
                            cout<<"Errors logged during file creation:"<<endl;
                            for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                            enterToCont();
                            errorlog.clear();
                        }
                    } else cout<<"No problems loaded! No file created."<<endl;
                } else if (vtmp=="aes") {
                    cout<<"AES test."<<endl<<"The \"All Diagnostics\" running mode is recommended for this to prevent any memory leaks."<<endl;
                    //first test with random keys and IV
                    /*
                     TESTS:
                     multithreaded aes test with big amounts of data
                     single threaded aes test with big amounts of data
                     multiple password/iv test with medium amounts of data
                     */
                    cout<<"Section 1:AES-128-CBC test"<<endl;
                    cout<<"Test 1: Multiple password/iv test."<<endl;
                    testAes(4096, 8192, 1, 16);
                    cout<<"Test 2:Data test."<<endl;
                    testAes(16,1048576, 1, 16);
                    testAes(16,1048576, 2, 16);
                    testAes(16,1048576, 4, 16);
                    testAes(16,1048576, 8, 16);
                    testAes(16,1048576, 16, 16);
                    testAes(16,1048576, 32, 16);
                    cout<<endl<<"Section 2:AES-192-CBC test"<<endl;
                    cout<<"Test 1: Multiple password/iv test."<<endl;
                    testAes(4096, 8192, 1, 24);
                    cout<<"Test 2:Data test."<<endl;
                    testAes(16,1048576, 1, 24);
                    testAes(16,1048576, 2, 24);
                    testAes(16,1048576, 4, 24);
                    testAes(16,1048576, 8, 24);
                    testAes(16,1048576, 16, 24);
                    testAes(16,1048576, 32, 24);
                    cout<<endl<<"Section 3:AES-256-CBC test"<<endl;
                    cout<<"Test 1: Multiple password/iv test."<<endl;
                    testAes(4096, 8192, 1, 32);
                    cout<<"Test 2:Data test."<<endl;
                    testAes(16,1048576, 1, 32);
                    testAes(16,1048576, 2, 32);
                    testAes(16,1048576, 4, 32);
                    testAes(16,1048576, 8, 32);
                    testAes(16,1048576, 16, 32);
                    testAes(16,1048576, 32, 32);
                } else {
                    for (ll i=0;i<asmt.size();i++) {
                        string cmpTmp=asmt[i].name;
                        transform(cmpTmp.begin(),cmpTmp.end(),cmpTmp.begin(),::tolower);
                        if (vtmp==cmpTmp) {
                            chosenID=i;
                            target=asmt[chosenID].id;
                            vq=true;
                        }
                    }
                    bool proc=false;
                    if (!vq) {
                        string pkgpth=vtmp;
                        nml(pkgpth);
                        if (pkgpth.length()>5) {
                            if (pkgpth.substr(pkgpth.size()-5,5)==".hwfx") {
                                ifstream pkgverf(pkgpth);
                                if (pkgverf.good()) {
                                    pkgverf.close();
                                    grp=0;
                                    if (pkgpth.size()>=5) {
                                        if (pkgpth.substr(pkgpth.size()-5,5)!=".hwfx") {
                                            cout<<"Unidentified file. Do not tamper with file extensions!"<<endl;
                                        }
                                    } else cout<<"Unidentified file. Do not tamper with file extensions!"<<endl;
                                    if (!readHwfx(pkgpth,vq,target,chosenID,debugMd,aesKey,randname,db,asmt,errorlog)) cout<<"Error importing file!"<<endl;
                                    proc=true;
                                    if (errorlog.size()) {
                                        cout<<"File corrupted. Logged errors during file import:"<<endl;
                                        for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                                        vq=false;
                                        enterToCont();
                                        errorlog.clear();
                                        clc();
                                    }
                                    if (autosave) savedata();
                                }
                            }
                        }
                    }
                    if (!vq&&!proc) {
                        cout<<"Invalid response."<<endl;
                    }
                }
            }
        }
        if (vq) {
            bool useCache=false;
            clc();
            ifstream bundltest(db+target);
            if (!bundltest.good()) {
                cout<<"Error! File missing."<<endl;
                continue;
            }
            string hwfxVerf;
            bundltest>>hwfxVerf;
            bundltest.close();
            vector<string>probdes;
            string probname="";
            vector<string>probin;
            vector<string>probout;
            vector<vector<string>>outex;
            vector<vector<string>>outdata;
            vector<vector<string>>indata;
            vector<vector<string>>inex;
            if (hwfxVerf=="HwfxContents::START") {
                string pkgpth=db+target;
                vector<string>reLd;
                ifstream testCache(db+"cache"+target);
                if (testCache.good()) {
                    string tmpgetcache;
                    useCache=true;
                    while (!testCache.eof()) {
                        getline(testCache,tmpgetcache);
                        reLd.push_back(tmpgetcache);
                    }
                    testCache.close();
                } else {
                    bool vqDummy=0;
                    readHwfxCore(reLd,pkgpth,vqDummy,debugMd,aesKey,errorlog);
                }
                chrono::steady_clock::time_point grs=chrono::steady_clock::now();
                //read in all except exin and inset
                //use maps to verify exin and inset
                //then read
                if (reLd.size()!=0) {
                    for (ll i=0;i<reLd.size()-1;i++) {
                        if (reLd[i]=="HwfxContents::Title") {
                            for (i++;i<reLd.size();i++) {
                                if (reLd[i].size()>14) {
                                    if (reLd[i].substr(0,14)=="HwfxContents::") break;
                                }
                                probname=probname+reLd[i];
                            }
                            i--;
                        } else if (reLd[i]=="HwfxContents::Description") {
                            for (i++;i<reLd.size();i++) {
                                if (reLd[i].size()>14) {
                                    if (reLd[i].substr(0,14)=="HwfxContents::") break;
                                }
                                probdes.push_back(reLd[i]);
                            }
                            i--;
                        } else if (reLd[i]=="HwfxContents::InputDes") {
                            for (i++;i<reLd.size();i++) {
                                if (reLd[i].size()>14) {
                                    if (reLd[i].substr(0,14)=="HwfxContents::") break;
                                }
                                probin.push_back(reLd[i]);
                            }
                            i--;
                        } else if (reLd[i]=="HwfxContents::OutputDes") {
                            for (i++;i<reLd.size();i++) {
                                if (reLd[i].size()>14) {
                                    if (reLd[i].substr(0,14)=="HwfxContents::") break;
                                }
                                probout.push_back(reLd[i]);
                            }
                            i--;
                        }
                    }
                } else {
                    
                }
                if (debugMd) {
                    cout<<"Parsing stage 1 done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
                    grs=chrono::steady_clock::now();
                }
                map<ll,dtCont>exCnt;
                map<ll,dtCont>stCnt;
                for (ll i=0;i<reLd.size();i++) {
                    if (reLd[i].size()>18) {
                        if (reLd[i].substr(0,18)=="HwfxContents::EXIN") {
                            exCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                            exCnt[atoll(reLd[i].substr(18).c_str())].ipos=i;
                        } else if (reLd[i].substr(0,18)=="HwfxContents::EXOU") {
                            exCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                            exCnt[atoll(reLd[i].substr(18).c_str())].opos=i;
                        } else if (reLd[i].substr(0,18)=="HwfxContents::INST") {
                            stCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                            stCnt[atoll(reLd[i].substr(18).c_str())].ipos=i;
                        } else if (reLd[i].substr(0,18)=="HwfxContents::OUST") {
                            stCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                            stCnt[atoll(reLd[i].substr(18).c_str())].opos=i;
                        }
                    }
                }
                ll rbndex=0;
                for (map<ll,dtCont>::iterator it=exCnt.begin();it!=exCnt.end();it++) {
                    if (it->second.cnt!=2) errorlog.push_back("EXSET ("+to_string(it->first)+")");;
                    rbndex=max(rbndex,it->first);
                }
                if (rbndex!=exCnt.size()) errorlog.push_back("EXMS ("+to_string(exCnt.size())+", "+to_string(rbndex)+")");
                for (ll i=1;i<=rbndex;i++) {
                    vector<string>exTmp;
                    for (ll j=exCnt[i].ipos+1;j<reLd.size();j++) {
                        if (reLd[j].find("HwfxContents::")!=string::npos) break;
                        exTmp.push_back(reLd[j]);
                    }
                    inex.push_back(exTmp);
                    exTmp.clear();
                    for (ll j=exCnt[i].opos+1;j<reLd.size();j++) {
                        if (reLd[j].find("HwfxContents::")!=string::npos) break;
                        exTmp.push_back(reLd[j]);
                    }
                    outex.push_back(exTmp);
                }
                
                if (!useCache) {
                    ll rbndst=0;
                    for (map<ll,dtCont>::iterator it=stCnt.begin();it!=stCnt.end();it++) {
                        if (it->second.cnt!=2) errorlog.push_back("IOSET ("+to_string(it->first)+")");;
                        rbndst=max(rbndst,it->first);
                    }
                    if (rbndst!=stCnt.size()) errorlog.push_back("SETMS ("+to_string(stCnt.size())+", "+to_string(rbndst)+")");
                    for (ll i=1;i<=rbndst;i++) {
                        vector<string>stTmp;
                        for (ll j=stCnt[i].ipos+1;j<reLd.size();j++) {
                            if (reLd[j].size()>=14) if (reLd[j].substr(0,14)=="HwfxContents::") break;
                            stTmp.push_back(reLd[j]);
                        }
                        indata.push_back(stTmp);
                        stTmp.clear();
                        for (ll j=stCnt[i].opos+1;j<reLd.size();j++) {
                            if (reLd[j].size()>=14) if (reLd[j].substr(0,14)=="HwfxContents::") break;
                            stTmp.push_back(reLd[j]);
                        }
                        outdata.push_back(stTmp);
                    }
                }
                if (debugMd) {
                    cout<<"Parsing stage 2 done in "<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - grs).count()<<"ms"<<endl;
                }
            } else {
                errorlog.push_back("STARTMISS");
            }
            if (errorlog.size()) {
                //remove the file
                bool sccode=false;
                ifstream scodetst(db+target+".cpp");
                if (scodetst.good()) sccode=true;
                scodetst.close();
                
                asmt.erase(asmt.begin()+chosenID);
                remove((db+target).c_str());
                if (sccode) remove((db+target+".cpp").c_str());
                ifstream testCache(db+"cache"+target);
                if (testCache.good()) {
                    testCache.close();
                    remove((db+"cache"+target).c_str());
                }
                
                cout<<"File error! This file may be corrupted."<<endl<<"Try redownloading the file from the source. If there is no problem, report the error log below:"<<endl;
                for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                errorlog.clear();
                enterToCont();
                
                clc();
                continue;
            }
            while (true) {
                cout<<endl<<probname<<endl<<"Your current score:"<<asmt[chosenID].score<<endl<<"--------"<<endl;
                for (ll i=0;i<probdes.size();i++) {
                    cout<<probdes[i]<<endl;
                }
                cout<<"--------"<<endl<<"Input:"<<endl;
                if (probin.size()) {
                    for (ll i=0;i<probin.size();i++) {
                        cout<<probin[i]<<endl;
                    }
                } else cout<<"None"<<endl;
                cout<<"--------"<<endl<<"Output:"<<endl;
                if (probout.size()) {
                    for (ll i=0;i<probout.size();i++) {
                        cout<<probout[i]<<endl;
                    }
                } else cout<<"None"<<endl;
                for (ll i=0;i<inex.size();i++) {
                    cout<<"--------"<<endl<<"Example input "<<i+1<<':'<<endl;
                    for (ll j=0;j<inex[i].size();j++) {
                        cout<<inex[i][j]<<endl;
                    }
                    cout<<"Example output "<<i+1<<':'<<endl;
                    for (ll j=0;j<outex[i].size();j++) {
                        cout<<outex[i][j]<<endl;
                    }
                }
                ll cumsub=1;
                ll delchc=-1,subscclc=-1,exitclc=-1,ansfclc=-1,subm=-1;
                cout<<"--------"<<endl;
                if (compilerExists&&canLaunch) {
                    cout<<"["<<cumsub++<<"]Submit"<<endl;
                    subm=cumsub-1;
                } else {
                    cout<<"Submissions are disabled (";
                    if (!compilerExists&&!canLaunch) {
                        cout<<"helper launch failed and compiler not set)"<<endl;
                    } else if (!compilerExists) {
                        cout<<"compiler not set)"<<endl;
                    } else {
                        cout<<"helper launch failed)"<<endl;
                    }
                }
                if (anslock&&compilerExists&&canLaunch) {
                    string simplansf=lockedf;
                    while (simplansf.find("/")!=string::npos) simplansf=simplansf.substr(simplansf.find("/")+1);
                    cout<<"["<<cumsub++<<"]Use answer file ("<<simplansf<<")"<<endl;
                    ansfclc=cumsub-1;
                }
                cout<<"["<<cumsub++<<"]Delete"<<endl;
                delchc=cumsub-1;
                bool sccode=false;
                ifstream scodetst(db+target+".cpp");
                if (scodetst.good()) sccode=true;
                scodetst.close();
                if (sccode&&SYS==APPL) {
                    cout<<"["<<cumsub++<<"]View last submitted source code"<<endl;
                    subscclc=cumsub-1;
                }
                cout<<"["<<cumsub++<<"]Exit"<<endl;
                exitclc=cumsub-1;
                string hwsubans;
                getline(cin,hwsubans);
                if (hwsubans!="-1") {
                    bool iscpf=false;
                    string tryd=hwsubans;
                    nml(tryd);
                    if (tryd.size()>4) {
                        if (tryd.substr(tryd.size()-4,4)==".cpp") {
                            ifstream makesure(tryd);
                            if (makesure.good()) {
                                iscpf=true;
                                makesure.close();
                            }
                        }
                    }
                    if (hwsubans==to_string(subm)||(anslock&&hwsubans==to_string(ansfclc))||(iscpf&&subm!=-1)) {
                        ofstream silence(db+"tmp/sl.silence");
                        if (!silence.good()) cout<<"Critical error! Error accessing TMP"<<endl;
                        silence<<"Silent!";
                        silence.close();
                        removeWithinFolder(db+"tmp/");
                        string cdpth;
                        if (!iscpf) {
                            if (hwsubans==to_string(ansfclc)) {
                                cdpth=lockedf;
                            } else {
                                clc();
                                cout<<"Please drag the cpp file into this window."<<endl;
                                getline(cin,cdpth);
                            }
                        } else cdpth=hwsubans;
                        
                        nml(cdpth);
                        ifstream codeverf(cdpth);
                        if (codeverf.good()) {
                            codeverf.close();
                            fcopy(cdpth, db+"tmp/preppedcode.cpp");
                            fcopy(cdpth,db+target+".cpp");
                            //MARK:Compile
                            string compilerArg="";
                            if (prefersCompiler) {
                                compilerArg=safespace(preferredCompiler);
                            } else {
                                if (SYS==APPL) compilerArg=safespace(compilers[defaultCompilerID].pth);
                                else if (SYS==WIN) compilerArg=safespace(compilers[defaultCompilerID].pth);
                            }
                            compilerArg+=" ";
                            string codepth=db+"tmp/preppedcode.cpp";
                            string execpth=db+"tmp/exe"+exePost;
                            stringstream reldinnerpth;
                            if (SYS==APPL) {
                                for (ll i=0;i<codepth.size();i++) codepth[i]=='"'?reldinnerpth<<"\\"<<codepth[i]:reldinnerpth<<codepth[i];
                                codepth=reldinnerpth.str();
                                reldinnerpth.str("");
                                for (ll i=0;i<execpth.size();i++) execpth[i]=='"'?reldinnerpth<<"\\"<<execpth[i]:reldinnerpth<<execpth[i];
                                execpth=reldinnerpth.str();
                                codepth="\""+safespace(codepth)+"\"";
                                execpth="\""+safespace(execpth)+"\"";
                            } else if (SYS==WIN) {
                                codepth=safespace(codepth);
                                execpth=safespace(execpth);
                            }
                            if (prefersCompiler) {
                                string toAdd="";
                                for (ll i=0;i<compilerFormat.size();i++) {
                                    if (i<=compilerFormat.size()-10) {
                                        if (compilerFormat.substr(i,10)=="%CODEPATH%") {
                                            toAdd+=codepth;
                                            i+=10-1;
                                        } else if (compilerFormat.substr(i,10)=="%EXECPATH%") {
                                            toAdd+=execpth;
                                            i+=10-1;
                                        } else toAdd+=compilerFormat[i];
                                    } else toAdd+=compilerFormat[i];
                                }
                                compilerArg+=toAdd;
                            } else {
                                //"%CODEPATH% -o %EXECPATH%"
                                compilerArg+=codepth;
                                compilerArg+=" -o ";
                                compilerArg+=execpth;
                            }
                            if (SYS==WIN) {
                                compilerArg="\""+compilerArg+"\"";
                            }
                            ofstream gppbrd(db+"tmp/gppbrd.bridge");
                            gppbrd<<compilerArg;
                            gppbrd.close();
//                            cout<<"\""+db+"err"+"\" \""+compilerArg+"\""<<endl;
                            inAppLaunch(db+"gpphelper"+exePost,"");
                            bool compile=true;
                            ifstream testcomp(db+"tmp/exe"+exePost);
                            if (testcomp.good()) compile=true;
                            else compile=false;
                            testcomp.close();
                            if (compile) {
                                if (useCache) {
                                    useCache=false;
                                    string pkgpth=db+target;
                                    vector<string>reLd;
                                    bool vqDummy=0;
                                    readHwfxCore(reLd,pkgpth,vqDummy,debugMd,aesKey,errorlog);
                                    map<ll,dtCont>stCnt;
                                    for (ll i=0;i<reLd.size();i++) {
                                        if (reLd[i].size()>18) {
                                            if (reLd[i].substr(0,18)=="HwfxContents::INST") {
                                                stCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                                                stCnt[atoll(reLd[i].substr(18).c_str())].ipos=i;
                                            } else if (reLd[i].substr(0,18)=="HwfxContents::OUST") {
                                                stCnt[atoll(reLd[i].substr(18).c_str())].cnt++;
                                                stCnt[atoll(reLd[i].substr(18).c_str())].opos=i;
                                            }
                                        }
                                    }
                                    ll rbndst=0;
                                    for (map<ll,dtCont>::iterator it=stCnt.begin();it!=stCnt.end();it++) {
                                        if (it->second.cnt!=2) cout<<"Error with i/o set #"<<it->first<<endl;
                                        rbndst=max(rbndst,it->first);
                                    }
                                    if (rbndst!=stCnt.size()) cout<<"Missing sets! "<<stCnt.size()<<" sets found, "<<rbndst<<" sets expected."<<endl;
                                    for (ll i=1;i<=rbndst;i++) {
                                        vector<string>stTmp;
                                        for (ll j=stCnt[i].ipos+1;j<reLd.size();j++) {
                                            if (reLd[j].size()>=14) if (reLd[j].substr(0,14)=="HwfxContents::") break;
                                            stTmp.push_back(reLd[j]);
                                        }
                                        indata.push_back(stTmp);
                                        stTmp.clear();
                                        for (ll j=stCnt[i].opos+1;j<reLd.size();j++) {
                                            if (reLd[j].size()>=14) if (reLd[j].substr(0,14)=="HwfxContents::") break;
                                            stTmp.push_back(reLd[j]);
                                        }
                                        outdata.push_back(stTmp);
                                    }
                                    if (indata.size()==0) errorlog.push_back("iosz0");
                                    if (errorlog.size()) {
                                        cout<<"Errors logged during cache load:"<<endl;
                                        for (ll i=0;i<errorlog.size();i++) cout<<errorlog[i]<<endl;
                                        enterToCont();
                                        errorlog.clear();
                                    }
                                }
                                //run multicore
                                ll folders=min((ll)indata.size(),mltcore);
                                for (ll i=1;i<=folders;i++) {
                                    ifstream testExist(db+"tmp/"+to_string(i)+"/exists");
                                    if (!testExist.good()) {
                                        makeDir(db+"tmp/"+to_string(i));
                                        ofstream markExist(db+"tmp/"+to_string(i)+"/exists");
                                        markExist.close();
                                    }
                                }
                                queue<ll>rmtasks;
                                auto checkAck = chrono::high_resolution_clock::now();
                                for (ll i=1;i<=folders;i++) {
                                    ofstream getAck(db+"tmp/"+to_string(i)+"/pth.bridge");
                                    getAck<<"ACK";
                                    getAck.close();
                                }
                                for (ll i=1;i<=indata.size();i++) {
                                    rmtasks.push(i);
                                }
                                //Copy over exe
                                for (ll i=1;i<=folders;i++) {
                                    copyf(db+"tmp/exe"+exePost,db+"tmp/"+to_string(i)+"/hwexe"+to_string(i)+exePost,0);
                                }
                                //Start the first batch
                                chrono::high_resolution_clock::time_point timer[folders];
                                bool rcvAck[folders];
                                for (ll i=0;i<folders;i++) rcvAck[i]=0;
                                ll rcvCum=0;
                                
                                while (true) {
                                    for (ll i=1;i<=folders;i++) {
                                        if (!rcvAck[i-1]) {
                                            ifstream lookAck(db+"tmp/"+to_string(i)+"/ack.bridge");
                                            if (lookAck.good()) {
                                                string lookfor;
                                                getline(lookAck,lookfor);
                                                if (lookfor=="ACK") {
                                                    rcvCum++;
                                                    rcvAck[i-1]=1;
                                                }
                                            }
                                        }
                                    }
                                    if (rcvCum==folders) break;
                                    if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - checkAck).count()>100) {
                                        for (ll i=1;i<=folders;i++) {
                                            if (!rcvAck[i-1]) {
                                                //Copy over openhelper
                                                copyf(db+"openhelper",db+"tmp2/openhelper"+to_string(i)+exePost,0);
                                                //Open openhelper
                                                //Write to the bridge
                                                ofstream foldout(db+"tmp/pth.bridge");
                                                if (!foldout.good()) cout<<"Critical error! Error accessing path bridge in tmp."<<endl;
                                                foldout<<i;
                                                foldout.close();
                                                launchNewProcess(db+"tmp2/openhelper"+to_string(i)+exePost);
                                                while (true) {
                                                    ifstream signaltest(db+"tmp/"+to_string(i)+"/firini.signal");
                                                    if (signaltest.good()) {
                                                        signaltest.close();
                                                        break;
                                                    }
                                                    usleep(1e4);
                                                }
                                            }
                                        }
                                        break;
                                    }
                                    usleep(1e4);
                                }
                                //MARK:START
                                for (ll i=1;i<=folders;i++) {
                                    ll curtask=rmtasks.front();
                                    rmtasks.pop();
                                    ofstream foldout;
                                    //Write in files
                                    foldout.open(db+"tmp/"+to_string(i)+"/in.txt");
                                    if (!foldout.good()) cout<<"Critical error! Error accessing in.txt in "<<to_string(i)<<endl;
                                    if (indata[curtask-1].size()!=0) foldout<<indata[curtask-1][0];
                                    for (ll j=1;j<indata[curtask-1].size();j++) {
                                        foldout<<endl<<indata[curtask-1][j];
                                    }
                                    foldout.close();
                                    foldout.open(db+"tmp/"+to_string(i)+"/pth.bridge");
                                    if (!foldout.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<"."<<endl;
                                    foldout<<"COM"<<endl<<db<<"tmp/"<<i<<"/"<<endl<<curtask<<endl<<db<<"tmp/"<<endl<<"hwexe"<<i<<exePost;
                                    foldout.close();
                                    //Wait for ini.signal
                                    while (true) {
                                        ifstream signaltest(db+"tmp/"+to_string(i)+"/ini.signal");
                                        if (signaltest.good()) {
                                            signaltest.close();
                                            break;
                                        }
                                        usleep(1e4);
                                    }
                                    timer[i-1]=chrono::high_resolution_clock::now();
                                    ofstream cle(db+"tmp/"+to_string(i)+"/pth.bridge");
                                    if (!cle.good()) cout<<"Critical error! Error clearing path bridge in "<<i<<"."<<endl;
                                    cle.close();
                                }
                                bool mainsig[folders];
                                for (ll i=1;i<=folders;i++) {
                                    mainsig[i-1]=false;
                                }
                                while (!rmtasks.empty()) {
                                    for (ll i=1;i<=folders;i++) {
                                        if (!mainsig[i-1]) {
                                            //Look for TE
                                            //Look for main.signal
                                            ifstream mainsigtest(db+"tmp/"+to_string(i)+"/main.signal");
                                            if (mainsigtest.good()) {
                                                string legit;
                                                mainsigtest>>legit;
                                                if (legit=="COM") {
                                                    mainsig[i-1]=true;
                                                    continue;
                                                }
                                            }
                                            if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - timer[i-1]).count()>1000) {
                                                stopProcess("hwexe"+to_string(i)+exePost);
                                                mainsig[i-1]=true;
                                            }
                                        }
                                    }
                                    for (ll i=1;i<=folders;i++) {
                                        if (rmtasks.empty()) break;
                                        if (mainsig[i-1]) { //if one of them are available...
                                            mainsig[i-1]=false;
                                            ofstream cle(db+"tmp/"+to_string(i)+"/main.signal");
                                            if (!cle.good()) cout<<"Critical error! Error clearing main.signal in "<<i<<endl;
                                            cle.close();
                                            cle.open(db+"tmp/"+to_string(i)+"/ini.signal");
                                            if (!cle.good()) cout<<"Critical error! Error clearing ini.signal in "<<i<<endl;
                                            cle.close();
                                            ll curtask=rmtasks.front();
                                            rmtasks.pop();
                                            //Write in files
                                            ofstream foldout;
                                            foldout.open(db+"tmp/"+to_string(i)+"/in.txt");
                                            if (!foldout.good()) cout<<"Critical error! Error accessing in.txt in "<<i<<endl;
                                            if (indata[curtask-1].size()!=0) foldout<<indata[curtask-1][0];
                                            for (ll j=1;j<indata[curtask-1].size();j++) {
                                                foldout<<endl<<indata[curtask-1][j];
                                            }
                                            foldout.close();
                                            //Write to the bridge
                                            foldout.open(db+"tmp/"+to_string(i)+"/pth.bridge");
                                            if (!foldout.good()) cout<<"Critical error! Error accessing path bridge in "<<i<<endl;
                                            foldout<<"COM"<<endl<<db<<"tmp/"<<i<<"/"<<endl<<curtask<<endl<<db<<"tmp/"<<endl<<"hwexe"<<i<<exePost;
                                            foldout.close();
                                            //Wait for ini.signal
                                            while (true) {
                                                ifstream signaltest(db+"tmp/"+to_string(i)+"/ini.signal");
                                                string legit;
                                                if (signaltest.good()) {
                                                    getline(signaltest,legit);
                                                    if (legit=="Ini") break;
                                                    signaltest.close();
                                                }
                                                usleep(1e4);
                                            }
                                            timer[i-1]=chrono::high_resolution_clock::now();
                                        }
                                    }
                                    usleep(1e4);
                                }
                                while (true) {
                                    bool canbk=true;
                                    for (ll i=1;i<=folders;i++) {
                                        if (!mainsig[i-1]) {
                                            canbk=false;
                                            //Something is NOT DONE AND NOT TERMINATED
                                            //Look for TE
                                            ifstream mainsigtest(db+"tmp/"+to_string(i)+"/main.signal");
                                            if (mainsigtest.good()) {
                                                string legit;
                                                getline(mainsigtest,legit);
                                                if (legit=="COM") {
                                                    mainsig[i-1]=true;
                                                    mainsigtest.close();
                                                    continue;
                                                }
                                            }
                                            mainsigtest.close();
                                            if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - timer[i-1]).count()>1500) {
                                                stopProcess("hwexe"+to_string(i)+exePost);
                                                mainsig[i-1]=true;
                                            }
                                        }
                                    }
                                    if (canbk) break;
                                    usleep(1e4);
                                }
                                
                                //Compare output
                                ll timeres[indata.size()];
                                for (ll i=1;i<=indata.size();i++) {
                                    ifstream timeread(db+"tmp/time"+to_string(i)+".txt");
                                    if (!timeread.good()) timeres[i-1]=10000;
                                    else timeread>>timeres[i-1];
                                    timeread.close();
                                }
                                ll correct=0;
                                clc();
                                cout<<"Test results:"<<endl;
                                //ofstream debg("hi.txt");
                                for (ll i=1;i<=indata.size();i++) {
                                    if (timeres[i-1]>1000) {
                                        cout<<"#"<<i<<"-TE"<<endl;
                                    } else {
                                        cout<<"#"<<i;
                                        //Evaluation:Remove last blank line
                                        vector<string>outcomp;
                                        ifstream outputc(db+"tmp/out"+to_string(i)+".txt");
                                        if (outputc.good()) {
                                            while (!outputc.eof()) {
                                                string tmp;
                                                getline(outputc,tmp);
                                                outcomp.push_back(tmp);
                                            }
                                            if (outcomp[outcomp.size()-1]=="") outcomp.erase(outcomp.begin()+outcomp.size()-1);
                                            for (ll j=0;j<outcomp.size();j++) {
                                                if (outcomp[j][outcomp[j].size()-1]=='\r') {
                                                    outcomp[j].erase(outcomp[j].begin()+outcomp[j].size()-1);
                                                }
                                                if (outcomp[j][outcomp[j].size()-1]==' ') {
                                                    outcomp[j].erase(outcomp[j].begin()+outcomp[j].size()-1);
                                                }
                                            }
                                            if (outdata[i-1][outdata[i-1].size()-1]=="") outdata[i-1].erase(outdata[i-1].begin()+outdata[i-1].size()-1);
                                            for (ll j=0;j<outdata[i-1].size();j++) {
                                                if (outdata[i-1][j][outdata[i-1][j].size()-1]=='\r') {
                                                    outdata[i-1][j].erase(outdata[i-1][j].begin()+outdata[i-1][j].size()-1);
                                                }
                                                if (outdata[i-1][j][outdata[i-1][j].size()-1]==' ') {
                                                    outdata[i-1][j].erase(outdata[i-1][j].begin()+outdata[i-1][j].size()-1);
                                                }
                                            }
                                            bool ac = true;
                                            /*
                                             //debug stuf
                                            debg<<endl<<endl<<"Stuf "<<i<<endl;
                                            debg<<"Start CADATA"<<endl;
                                            for (ll j=0;j<outdata[i-1].size();j++) {
                                                for (ll k=0;k<outdata[i-1][j].size();k++) {
                                                    debg<<(int)outdata[i-1][j][k]<<' ';
                                                }
                                                debg<<endl;
                                            }
                                            debg<<"Start USERDATA"<<endl;
                                            for (ll j=0;j<outcomp.size();j++) {
                                                for (ll k=0;k<outcomp[j].size();k++) {
                                                    debg<<(int)outcomp[j][k]<<' ';
                                                }
                                                debg<<endl;
                                            }
                                             */
                                            if (outdata[i-1].size()==outcomp.size()) {
                                                for (ll j=0;j<outdata[i-1].size();j++) {
                                                    if (outdata[i-1][j].size()==outcomp[j].size()) {
                                                        for (ll k=0;k<outdata[i-1][j].size();k++) {
                                                            if (outdata[i-1][j][k]!=outcomp[j][k]) {
                                                                ac=false;
                                                                break;
                                                            }
                                                        }
                                                    } else ac=false;
                                                }
                                            } else ac = false;
                                            if (ac) {cout<<"-AC:";correct++;}
                                            else cout<<"-WA:";
                                            cout<<timeres[i-1]<<"ms"<<endl;
                                        } else cout<<"-RTE"<<endl;
                                    }
                                }
//                                debg.close();
                                removeWithinFolder(db+"tmp/");
                                ll score = round(((ld)correct/indata.size())*100.0);
                                if (asmt[chosenID].score>score) cout<<"Your submission score:"<<score<<endl;
                                asmt[chosenID].score=max(asmt[chosenID].score,score);
                                cout<<"Your current score:"<<asmt[chosenID].score<<endl;
                                if (score==100) {
                                    cor++;
                                    if (cor==1) cout<<"Achievement \"Success\" complete."<<endl;
                                    else if (cor==10) cout<<"Achievement \"More!\" complete."<<endl;
                                    else if (cor==100) cout<<"Achievement \"Unstoppable\" complete."<<endl;
                                    else if (cor==1000) cout<<"Achievement \"Godlike!\" complete."<<endl;
                                    if (sound) {
                                        if (selaud.size()>0) {
                                            ofstream songfname(db+"songbrd");
                                            songfname<<audio[selaud[rand()%selaud.size()]].fname;
                                            launchNewProcess(db+"acsound"+exePost);
                                        }
                                    }
                                } else {
                                    incor++;
                                    if (incor==1) cout<<"Achievement \"You can try again\" complete."<<endl;
                                    else if (incor==10) cout<<"Achievement \"Get up!\" complete."<<endl;
                                    else if (incor==100) cout<<"Achievement \"Keep going!\" complete."<<endl;
                                    else if (incor==1000) cout<<"Achievement \"Nearly there!\" complete."<<endl;
                                }
                                if (autosave) savedata();
                                cout<<"Press return to continue...";
                                cin.get();
                                if (SYS==APPL) stopProcess("acsound"+exePost);
                                clc();
                            } else {
                                cout<<"Compilation error."<<endl;
                            }
                        } else {
                            cout<<"Code error!";
                        }
                    } else if (hwsubans==to_string(delchc)) {
                        asmt.erase(asmt.begin()+chosenID);
                        remove((db+target).c_str());
                        if (sccode) remove((db+target+".cpp").c_str());
                        ifstream testCache(db+"cache"+target);
                        if (testCache.good()) {
                            testCache.close();
                            remove((db+"cache"+target).c_str());
                        }
                        clc();
                        break;
                    } else if (hwsubans==to_string(subscclc)&&sccode) {
                        system(("open -a Finder "+safespace(db+target+".cpp")).c_str());
                    } else if (hwsubans==to_string(exitclc)) {
                        clc();
                        break;
                    } else {
                        clc();
                        cout<<"Invalid response."<<endl;
                    }
                } else cout<<"Invalid response."<<endl;
            }
        }
    }
}
