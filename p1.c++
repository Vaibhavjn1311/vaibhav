#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cerrno>
#include <unistd.h>
#include <cstring>
using namespace std;

unsigned long long total;
unsigned long long current = 0;


#define block 1024

// void progress_bar(int progress){
//         int bar_width = 10;
//         cout<<"[";
//         int pos = bar_width * progress;
//         if(int i=0;i<1024;i++){
//             if (i < pos) cout << "=";
//             else if (i == pos) cout << ">";
//             else cout << " ";
//         }
// }

void reverse(int output_fd,char* buffer,int k){
    char newText[k];
    // cout<<buffer<<endl;
    for(int i=0;i<k;i++){
        newText[i] = buffer[k-i-1];
    }
    // cout<<newText;

    write(output_fd,newText,k);

    current = current+k;

    cout<<"\r"<<"Progress : "<< (current*100)/total<<"%"<<flush;
    
   
}


int main(int argc, char *argv[]) {
    if (argc < 3 || (std::stoi(argv[2]) == 1 && argc != 5)) {
        cerr << "Parameter not passed properly";
        return EXIT_FAILURE;
    }

    char* inputFile = argv[1];
    int flag = stoi(argv[2]);
    off_t startIndex = 0, endIndex = 0;

    

    
    if (flag == 1) {
        startIndex = stoi(argv[3]);
        endIndex = stoi(argv[4]);
    }
    const char* dirPath = "Assigement1";
    const char* dirPath2;
    if(flag==0){
         dirPath2 = "Assigement1/0_";
    }

    else if(flag==1){
         dirPath2 = "Assigement1/1_";
    }
    mode_t mode = 0755;

    int result = mkdir(dirPath, mode);

    if (result != 0) {
       // cerr << "Error creating directory: " << strerror(errno) << std::endl;;
    } 

    int len = strlen(argv[1]);
    
    char* pos = new char[len+15];
    strcat(pos,dirPath2);
    strcat(pos,argv[1]);

    

    int output_fd = open(pos,O_WRONLY | O_CREAT | O_APPEND | O_TRUNC,0600);
    
    int input_fd = open(inputFile,O_RDONLY);

    char buffer[block];

    

    unsigned long long eof = lseek(input_fd,0,SEEK_END);

    cout<<eof<<endl;

    total = eof-1;
    
    int k;

    if(flag==0){

        long long temp = eof;
        int n=1;
        
        while(temp>=0){

            if(temp<=block){
                lseek(input_fd,0,SEEK_SET); 
                k = read(input_fd,buffer,temp);
                reverse(output_fd,buffer,k); 
                temp=-1;
            }
            else{
                lseek(input_fd,-(n*block),SEEK_END);                
                k = read(input_fd,buffer,block);
                reverse(output_fd,buffer,k); 
                n++;
                
                temp = temp - block;

            }
        
        }

    }

    else if(flag==1){

            int n = 1;
            long long start = lseek(input_fd,startIndex,SEEK_SET);

            while(start>=0){
                if(start < block){
                    lseek(input_fd,0,SEEK_SET);
                    k = read(input_fd,buffer,start)-1;
                    reverse(output_fd,buffer,k+1);
                    start = -1;
                }
                else{
                    lseek(input_fd,-(n*block),SEEK_CUR);
                    k = read(input_fd,buffer,block);
                    reverse(output_fd,buffer,k);
                    n++;
                    start = start-block;
                }
            }
            
            long long curr = lseek(input_fd,startIndex,SEEK_SET);
            
            while(curr<endIndex){
                if(endIndex-startIndex < block){
                    k = read(input_fd,buffer,endIndex-startIndex);
                }
                else{
                    k = read(input_fd,buffer,block);
                }
                write(output_fd,buffer,k);
                curr = curr + k;

                current = current+k;

                cout<<"\r"<<"Progress : "<< (current*100)/total<<"%"<<flush;
            }

            n=1;
            long long temp = eof-endIndex;
            
            while(temp>=0){
                
                if(temp <= block){
                    lseek(input_fd,endIndex,SEEK_SET);
                    k = read(input_fd,buffer,temp);
                    reverse(output_fd,buffer,k);
                    temp = -1;
                }
                else{
                    lseek(input_fd,-(n*block),SEEK_END);
                    k = read(input_fd,buffer,block);
                    reverse(output_fd,buffer,k);
                    n++;
                    temp = temp -block;
                }

            }
    }

    return 0;
}

