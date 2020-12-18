#include<bits/stdc++.h>
#define e 2.718281828
#define MAXN 100
using namespace std;

vector< vector<float> > X,Y,T;
int input = 2,output = 3,h = 0;
vector<int> hiden;
void addhidenlayer()
{
    hiden.push_back(3);
  //  hiden.push_back(3);
}
float alp = 0.3;
float w[MAXN][MAXN];
float tt[MAXN];
ifstream fi("training2f.INP");
ifstream fii("testing2f.INP");

void training(vector< vector<float> > x, vector< vector<float> > Y)
{
    int d = input,ii = 1;
    for(int i = 0;i < hiden.size();i++)
    {
        int jj = d+1;
        while(ii <= d)
        {
          //  cout<<ii<<" "<<jj<<endl;
            w[ii][jj++] = 0;
            if(jj > d+hiden[i])jj = d+1,ii++;
        }
        d += hiden[i];
    }
    int jj = d+1;
    while(ii <= d)
    {
      //  cout<<ii<<" "<<jj<<endl;
        w[ii][jj++] = 1;
        if(jj > d+output)jj = d+1,ii++;
    }
    tt[input+1] = -0.3;
    for(int i = input+2;i <= input+output+h;i++)tt[i] = tt[i-1]+0.1;
    float gr[input+output+h+2];
    int n = 5000;
    while(n--)
    {
        for(int i = 0;i < x.size();i++)
        {
            d = input; ii = 1;
            float y[input+output+h+2];
            for(int k = 0;k < x[i].size();k++)y[k+1] = x[i][k];
            // Ket xuat thuc tang an
            int nut = 1;
            for(int k = 0;k < hiden.size();k++)
            {
                for(int l = 1;l <= hiden[k];l++)
                {

                    jj = d+l; ii = nut;
                    float s = 0;
                    while(ii <= d)
                    {
                      //  cout<<ii<<" "<<jj<<endl;
                        s += y[ii]*w[ii++][jj];
                    }
                   // cout<<jj<<endl;
                    y[jj] = 1/(1+pow(e,-(s-tt[jj])));
                }
                nut = d+1;
                d += hiden[k];
            }
            // Ket xuat thuc tang xuat
            for(int l = 1;l <= output;l++)
                {
                    jj = d+l; ii = nut;
                    float s = 0;
                    while(ii <= d)
                    {
                       // cout<<ii;
                        s += y[ii]*w[ii++][jj];
                    }
                   // cout<<jj<<endl;
                    y[jj] = 1/(1+pow(e,-(s-tt[jj])));
                  //  cout<<y[jj]<<endl;
                }
            // gradient sai so lop suat
            for(int j = 0;j < output;j++)
            {

                ii = d+j+1;
               // cout<<ii<<i<<j<<endl;
                gr[ii] = y[ii]*(1-y[ii])*(Y[i][j]-y[ii]);
            }

            // gradient sai so lop an
            ii = d;
            int layer = output;
            for(int k = hiden.size()-1;k >= 0;k--)
            {
                d -= hiden[k];
                while(ii > d)
                {
                    float s = 0;
                    for(int j = d+hiden[k]+1;j <=d+hiden[k]+layer;j++)
                        s += gr[j]*w[ii][j];
                    gr[ii] = y[ii]*(1-y[ii])*s;
                  //  cout<<gr[ii]<<endl;
                    ii--;
                }
                layer = hiden[k];
            }

            //cap nhat trong so
            ii = 1;d = input;
            for(int k = 0;k < hiden.size();k++)
            {
                jj = d+1;
                while(ii <= d)
                {
                  //  cout<<ii<<" "<<jj<<endl;
                    w[ii][jj] += alp*y[ii]*gr[jj];
                   //  cout<<w[ii][jj]<<" "<<y[ii]<<" "<<gr[jj]<<endl;
                    jj++;

                    if(jj > d+hiden[k])jj = d+1,ii++;
                }
                d += hiden[k];
            }

            jj = d+1;
            while(ii <= d)
            {
                // cout<<ii<<" "<<jj<<endl;
                w[ii][jj] += alp*y[ii]*gr[jj];
                jj++;
                if(jj > d+output)jj = d+1,ii++;
            }
            // cap nhat tt
            for(int k = input+1;k <= input+h+output;k++)
                tt[k] += (-1)*alp*gr[k];

        }
    }
}

void testing(vector< vector<float> > x)
{
  for (int i = 0; i<x.size(); i++)
    {
    float y[input+output+h+2];
    for(int k = 0;k < x[i].size();k++)y[k+1] = x[i][k];
    int d = input, ii = 1,jj,nut = 1;
    // Ket xuat thu tang an
    for(int k = 0;k < hiden.size();k++)
    {
        for(int l = 1;l <= hiden[k];l++)
        {
            jj = d+l; ii = nut;
            double s = 0;
            while(ii <= d)
            {
               // cout<<ii;
                s += y[ii]*w[ii++][jj];
            }
            //cout<<jj<<endl;
            y[jj] = 1/(1+pow(e,-(s-tt[jj])));
        }
        nut = d+1;
        d += hiden[k];
    }
    // Ket xuat thuc tang xuat
    for(int l = 1;l <= output;l++)
        {
            jj = d+l; ii = nut;
            double s = 0;
            while(ii <= d)
            {
              //  cout<<ii<<endl;
                s += y[ii]*w[ii++][jj];
            }
            y[jj] = 1/(1+pow(e,-(s-tt[jj])));
        }
    ii = input+h+1;
    for(int j = 0;j < x[i].size()-3;j++)cout<<x[i][j]<<" ";
    cout<<" -> ";

    if (y[ii] > y[ii+1] && y[ii] > y[ii+2]){
      cout<<"Ket qua: Iris-setosa  | ";
    }
    else if(y[ii+1] > y[ii] && y[ii+1] > y[ii+2]){
      cout<<"Ket qua: Iris-Versicolor  | ";
    }
    else{
      cout<<"Ket qua: Iris-Verginica  | ";
    }
    cout<<"Ket qua mong muon: ";
    if(x[i][input] == 1)cout<<"Iris-setosa"<<endl;
    else if(x[i][input+1] == 1)cout<<"Iris-Versicolor"<<endl;
    else cout<<"Iris-Verginica"<<endl;
  }
}

void getIris(){
	float x;
	while(fi>>x){
            vector < float > v,vv;
            v.push_back(x);
            for(int i = 2;i <= input;i++)fi>>x,v.push_back(x);
            for(int i = 1;i <= output;i++)fi>>x,vv.push_back(x);
            X.push_back(v);
            Y.push_back(vv);
	}
}


void getIrisTest(){
	float x;
	while(fii>>x){
        vector < float > v;
        v.push_back(x);
        for(int i = 2;i <= input;i++)fii>>x,v.push_back(x);
        for(int i = 1;i <= output;i++)fii>>x,v.push_back(x);
		T.push_back(v);
	}
}


int main(){
    addhidenlayer();
    for(int i = 0;i < hiden.size();i++)
    {
        h+=hiden[i];
    }
    getIris();
    getIrisTest();
    training(X, Y);
    testing(T);
    fi.close();
    fii.close();
}

