#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <time.h>
#include <chrono>
#include <gcrypt.h>
using namespace std;

void gcrypt_init()
{
    if (!gcry_check_version ("1.9.2"))
    {
        printf("gcrypt: library version mismatch");
    }
    gcry_error_t err = 0;
    err = gcry_control (GCRYCTL_SUSPEND_SECMEM_WARN);
    err |= gcry_control (GCRYCTL_INIT_SECMEM, 102400, 0);
    err |= gcry_control (GCRYCTL_RESUME_SECMEM_WARN);
    err |= gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
    if (err) {
        printf("gcrypt: failed initialization");
    }
}

int main() {
    gcrypt_init();
    
    printf("RSA key generation ... \n\n");

    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;

    err = gcry_sexp_build(&rsa_parms, NULL, "(genkey (rsa (nbits 3:512)))");
    if (err) {
        printf("gcrypt: failed to create rsa params");
    }

    int n=30;
    double exec_time[n];

    for(int i=0;i<n;i++)
    {
        auto start = chrono::high_resolution_clock::now();
        ios_base::sync_with_stdio(false);
        err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
        auto end = chrono::high_resolution_clock::now();

        if (err) {
            printf("gcrypt: failed to create rsa key pair");
        }

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        time_taken *= 1e-9;
        cout << "Time taken by program is : " << fixed << time_taken;
        cout << " sec" << endl;
        exec_time[i]=time_taken;
    }
    double sum=0;
    for(int i=0;i<n;i++)
    {
        sum+=exec_time[i];
    }
    double avg=sum/n;
    cout<<"Average: "<<avg<<endl;
  }
