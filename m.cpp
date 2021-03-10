#include <iostream>
#include "./matrix.cpp"
using namespace std;

int main()
{

  Matrix::MATRIX_VERBOSE = true;      //shows some useful informations when true
  Matrix::MATRIX_PRINT_PRECISION = 4; //the precision of numbers when printed
  int i, j;
  Matrix mymatrix(3, 5), secondmatrix(3, 5); //declaring my matrix objects

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      mymatrix.set(i, j, (double)i * j + i - j);
      secondmatrix.set(i, j, (double)i * j * j - i);
    }
  }
  mymatrix.set(1, -1, 3.1415926535); //setting values into the object

  //"cout" prints a matrix
  cout << mymatrix;     // printing the matrix
  cout << mymatrix.T(); // transposing the matrix

  //operations with scalar
  cout << mymatrix + 5; //summing a scalar value
  cout << 5 + mymatrix; //summing a scalar value
  cout << mymatrix - 5; //subtracting a scalar value
  cout << 5 - mymatrix; //subtracted by scalar value
  cout << mymatrix * 5; //multiplying by a scalar value
  cout << 5 * mymatrix; //multiplying by a scalar value
  cout << mymatrix / 5; //dividing by a scalar value
  cout << 5 / mymatrix; //dividing a scalar value

  //operations with matrices "entry-to-entry"
  cout << mymatrix + secondmatrix; //summing a matrix
  cout << mymatrix - secondmatrix; //subtracting a matrix
  cout << mymatrix * secondmatrix; //multiplying by a matrix
  cout << mymatrix / secondmatrix; //dividing by a matrix

  //matrix multiplication of type "row dot column" with the "%" operator
  cout << mymatrix % secondmatrix.T(); //matrix multiplication with "%"

  //changing the sign of the matrix entries
  cout << -mymatrix;

  //All the boolean operators "!=, ==, >=, <=, >, <" are implemented
  //as examples, see below
  cout << (mymatrix >= secondmatrix); //1 when true, 0 when false
  cout << (4 >= mymatrix);
  cout << (mymatrix >= 4);

  cout << eye(6, 4);
  cout << mymatrix;
  secondmatrix.to_csv("2");
  mymatrix.to_csv("Nome");

  return 0;
}