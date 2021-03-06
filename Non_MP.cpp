#include <iostream>
#include <iomanip>
#include <vector>
#include <omp.h>
using namespace std;


int main()
{
   // Original data
   vector<double> xData;
   for(int i = 1; i <= 100000; i++){
      xData.push_back((int)i * 5);
   }
   vector<double> yData;
   double y = 0.3;
   for(int i = 1; i <= 100000; i++){
      y += 0.2;
      yData.push_back(y);
   }


   // Set up some points for interpolation in xVals
   const int NPTS = xData[xData.size() - 1];
   vector<double> xVals;
   for ( int i = 1; i <= NPTS; i++ ) xVals.push_back( (int)i );
   vector<double> yVals;
   for ( int i = 1; i <= NPTS; i++ ) yVals.push_back( 0 );

//    omp_set_num_threads (4);

  
//    int i;
//    #pragma omp parallel shared(xData, yData, xVals, yVals, size) private(i)
//    {

    //   #pragma omp for 
      // Interpolate
    for (int i = 0; i < NPTS; i++ )
    {
        int counter = 0;     
        int size = xData.size();                         // find left end of interval for interpolation
        if ( xVals[i] >= xData[size - 2] )               // special case: beyond right end
        {
            counter = size - 2;
        }
        else
        {
            while ( xVals[i] > xData[counter+1] ) counter++;
        }
        double xL = xData[counter], yL = yData[counter], xR = xData[counter+1], yR = yData[counter+1];
        if ( xVals[i] < xL ) {
            yR = yL;
        }
        if ( xVals[i] > xR ) {
            yL = yR;
        }
        double dydx = ( yR - yL ) / ( xR - xL );
        double y = yL + dydx * ( xVals[i] - xL );
        yVals[i] = y;
    }

//    }
   

   // Output
//    cout << "Original data:\n";
//    for ( int i = 0; i < xData.size(); i++ ) cout << fixed << setw( 15 ) << setprecision( 6 ) << xData[i] << fixed << setw( 15 ) << setprecision( 6 ) << yData[i] << '\n';
//    cout << "\nInterpolated data:\n";
//    for ( int i = 0; i < xVals.size(); i++ ) cout << fixed << setw( 15 ) << setprecision( 6 ) << xVals[i] << fixed << setw( 15 ) << setprecision( 6 ) << yVals[i] << '\n';
}
