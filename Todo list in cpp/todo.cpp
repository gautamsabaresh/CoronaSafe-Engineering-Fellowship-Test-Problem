#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include<iostream>
#include<string>
#include<ctime>
#include<fstream>
#include<vector>
using namespace std;
string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}
//Number of lines in a file
int numberoflines(string filename){
    vector<string> filelines;
    ifstream file;
        file.open(filename,ios::in);
        if (file.is_open()){
            string textfromfile;
            while (getline(file, textfromfile))
            {
                // Process str
                filelines.push_back(textfromfile);
            }
            file.close();
        }
    return filelines.size();
}
//delete a line from a file
void deletefromfile(int linenum){
        ifstream file;
        ofstream ofs;
        ofs.open("temp.txt", ofstream::out);
        file.open("todo.txt",ios::in);
        if (file.is_open()){
            char c;
            int line_no=1;
            while(file.get(c))
            {
               if (c == '\n')
               line_no++;
               // file content not to be deleted
               if (line_no != linenum)
               {
                  ofs << c;
                }
            }
            ofs.close();
            // closing input file
            file.close();
            // remove the original file
            remove("todo.txt");
            // rename the file
            rename("temp.txt", "todo.txt");
        }
        else{
            cout<<"Error on opening the file";
        }
}

void remove_blanklines(string filename){
    string filepathtodo=get_current_dir()+"\\todo.txt";
    ifstream fin(filename);
    ofstream fout;
    fout.open("temp.txt", ios::out);
    string str;
    while(getline(fin,str))
    {
        while (str.length()==0 )
            getline(fin,str);
        fout<<str<<endl;
    }
    fout.close();
    fin.close();
    remove("todo.txt");
    rename("temp.txt", "todo.txt");
}
int main(int argc, char** argv)
{
    string filepathtodo=get_current_dir()+"\\todo.txt";
    string filepathdone=get_current_dir()+"\\done.txt";
    const int MAXLEN = 80;
    char date[MAXLEN];
    time_t t = time(0);
    vector<string> todopending;
    vector<string> tododone;
    string func="";
    if(argc>1)
    func=argv[1];
    //help and no argument
    if(argc==1 || func.compare("help")==0){

        cout<<"Usage :-"<<endl;
        cout<<"$ ./todo add \"todo item\"  # Add a new todo"<<endl;
        cout<<"$ ./todo ls               # Show remaining todos"<<endl;
        cout<<"$ ./todo del NUMBER       # Delete a todo"<<endl;
        cout<<"$ ./todo done NUMBER      # Complete a todo"<<endl;
        cout<<"$ ./todo help             # Show usage"<<endl;
        cout<<"$ ./todo report           # Statistics";
    }
    //add
    if(func.compare("add")==0){
        if(argc!=3){
            cout<<"Error: Missing todo string. Nothing added!";
        }
        else{
        string text=argv[2];
        ofstream foutput;
        ifstream finput;
        finput.open ("todo.txt",ios::in);
        foutput.open ("todo.txt",ios::app);
            if ( finput.peek() == std::ifstream::traits_type::eof())//checking if Empty File
            {
                foutput<<text;
            }
            else
            foutput<<"\n"<<text;

        cout<<"\nAdded todo: "<<"\""<<text<<"\"";
        finput.close();
        foutput.close();
    }
    }
    //report
    if(func.compare("report")==0){
            int todopendingcount=numberoflines(filepathtodo);
            int count=numberoflines("done.txt");
            strftime(date, MAXLEN, "%Y-%m-%d", localtime(&t));
            cout << date <<" Pending : "<<todopendingcount<<" Completed : "<<count;
        }

    //ls
    if(func.compare("ls")==0){
          if(numberoflines(filepathtodo)>0){
                remove_blanklines("todo.txt");
            }
        ifstream file;
        file.open(filepathtodo,ios::in);
            if ( file.peek() == std::ifstream::traits_type::eof()){
                cout<<"There are no pending todos!";
                file.close();
            }else{
            string textfromfile;

            while (getline(file, textfromfile))
            {
                todopending.push_back(textfromfile);
            }
            file.close();
            int temp=todopending.size();
            for(int i=todopending.size()-1;i>=0;i--)
            {
                cout<<"["<<temp<<"] "<<todopending[i]<<endl;
                temp--;
            }
        }
    }
    //del
    if(func.compare("del")==0){
        int tobedeletednum=atoi(argv[2]);
        int count=numberoflines("todo.txt");
        if(tobedeletednum<=count && tobedeletednum!=0){
            if(numberoflines("todo.txt")>0){
                remove_blanklines("todo.txt");
            }
            deletefromfile(tobedeletednum);
            cout<<"Deleted todo #"<<tobedeletednum;
         }
        else{
                if(argc!=3){
                cout<<"Error: Missing NUMBER for deleting todo.";
                }
                else
            cout<<"Error: todo #"<<tobedeletednum<<" does not exist. Nothing deleted.";
        }
    }
    //done
    if(func.compare("done")==0){

        int count=numberoflines(filepathtodo);
        int tobecompletednum=atoi(argv[2]);

        ofstream ofs;
        string donetext;
        ofs.open(filepathdone, ofstream::app);

    if(tobecompletednum<=count && tobecompletednum!=0){

            ifstream file1;
        file1.open(filepathtodo,ios::in);
        if (file1.is_open()){
            char c;
            int line_no=1;

            while(file1.get(c))
            {
               if (c == '\n')
                line_no++;
                // file content to be added in done.txt
                if (line_no == tobecompletednum){
                strftime(date, MAXLEN, "%Y/%m/%d", localtime(&t));
                if(tobecompletednum==1){
                 file1.seekg(0);
                 getline(file1,donetext);
                }
                else{
                getline(file1,donetext);
                }
                ifstream ifs;
                ifs.open(filepathdone);
                 if ( ifs.peek() == std::ifstream::traits_type::eof())//checking if Empty File
                {
                    ofs<<"x "<<date<<" "<<donetext;
                }
                else{
                    ofs<<"\n"<<"x "<<date<<" "<<donetext;
                }
                break;
                }
            }
            file1.close();
            ofs.close();
            cout<<"Marked todo #"<<tobecompletednum<<" as done.";
            if(numberoflines(filepathtodo)>0)
                remove_blanklines(filepathtodo);
            deletefromfile(tobecompletednum);
            }
        else{
            cout<<"Error on opening file.";
        }

        }
    else{
            if(argc!=3){
            cout<<"Error: Missing NUMBER for marking todo as done.";
        }
    else
        cout<<"Error: todo #"<<tobecompletednum<<" does not exist.";
        }
    }
    if(func.compare("add")!=0 && func.compare("done")!=0 && func.compare("del")!=0 && func.compare("ls")!=0 && func.compare("report")!=0 && func.compare("help")!=0 && func.compare("")!=0){
        cout<<"Invalid function.";

    }
    return 0;
}
