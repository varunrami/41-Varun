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
    printf("RSA encryption ... \n\n");

    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;

    err = gcry_sexp_build(&rsa_parms, NULL, "(genkey (rsa (nbits 4:4096)))");
    if (err) {
        printf("gcrypt: failed to create rsa params");
    }

    err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
    if (err) {
        printf("gcrypt: failed to create rsa key pair");
    }
   
    gcry_mpi_t mpi_n,mpi_e,mpi_d,mpi_p,mpi_q,mpi_u;
    err=gcry_sexp_extract_param(rsa_keypair,NULL,"n e d p q u",&mpi_n,&mpi_e,&mpi_d,&mpi_p,&mpi_q,&mpi_u,NULL);
     if (err) {
        printf("gcrypt: failed to extract rsa keys");
    }
  
    gcry_sexp_t rsa_public;

    err = gcry_sexp_build(&rsa_public, NULL, "(public-key (rsa (n %M)(e %M)))",mpi_n,mpi_e);
    if (err) {
        printf("gcrypt: failed to create rsa public key");
    }
    char block[]="53";
 
    gcry_sexp_t rsa_plaindata;
    err = gcry_sexp_build(&rsa_plaindata, NULL, "(data (flags pkcs1)(value %s))",block);
    if (err) {
        printf("gcrypt: failed to create data sexp");
    }

    gcry_sexp_t encrypted_data;

    int n=1000;
    double exec_time[n];

    for(int i=0;i<n;i++)
    {
        auto start = chrono::high_resolution_clock::now();
        ios_base::sync_with_stdio(false);

        err=gcry_pk_encrypt(&encrypted_data,rsa_plaindata,rsa_public);
        if (err) {
            printf("gcrypt: failed to encrypt data ");
        }

        auto end = chrono::high_resolution_clock::now();

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
