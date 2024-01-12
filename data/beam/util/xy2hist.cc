#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

double interpolate(double x,int n,double X[],double Y[])
{
    /*
     * Given n points (X[i],Y[i]) and real x
     * find y such that (x,y) linearly interpolates one of the 
     * sections or y=Y[0] if x<=X[0] or y=Y[n-1] if x>=X[n-1]
     */
     
     
    if(x<=X[0]) 
        return Y[0];
    if(x>=X[n-1])
        return Y[n-1];
    int i=0;
    while(x>X[i+1]) 
        i++;
    double a=x-X[i];
    double b=X[i+1]-x;
    
    return (b*Y[i]+a*Y[i+1])/(a+b);

}

int main()
{
    /* 
     * convert energy profile
     * given as file of x y pairs (one per line)
     * to a one line histogram 
     * E_min E_max bin1 bin2 binN   
     * suitable for use as nuwro energy profile
     * Conversion of units GeV=>MeV must be 
     * performed maualy by multipying 
     * first two numbers of output by 1000.
     * 
     * E_min and E_max are calculated such that 
     * the center of first bin is x_1 and 
     * the center of last  bin is x_n
     * 
     * values bin1 ... binN  are values at bin centers 
     * linearly interpolated from the input points (xi, yi)
     * 
     * number of bins N = 2n-1 (n=number of input points)
     * you may use N = n if xi are equally spaced
     * but using much larger N will not harm you.
     * in the limit N->infinity the linear interpolation is obtained
     */
    vector<double> X,Y;
    double key,val;
    while(cin>>key>>val)
    {
        X.push_back(key);
        Y.push_back(val);
    }
    int n=Y.size();
    int N=n;
    N=2*n-1;  // comment this line out if bins are of equal size
    double x0=X[0];
    double x1=X[n-1];
    double dx=(x1-x0)/(N-1);
    cout<<x0-dx/2<<" "<<x1+dx/2;
    for(int i=0;i<N;i++)
    {
        double x=x0+dx*i;
        double y=interpolate(x, n, &X[0], &Y[0]);
        cout<<" "<<y;
    }
    cout<<endl;
}
