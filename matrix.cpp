#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <exception>
using namespace std;

class Matrix
{
private:
  /* data */
  int ncols = 0, nrows = 0;
  double **me = nullptr;
  int validate_index(int index, int i);
  void set_dims(int rows, int cols);

public:
  static bool MATRIX_VERBOSE;
  static int MATRIX_PRINT_PRECISION;
  static bool MATRIX_EXIT_IF_ERROR;
  /* methods */
  Matrix(int nrows, int ncols);
  Matrix();
  ~Matrix();
  double get(int i, int j);
  int rows();
  int cols();
  void set(int i, int j, double value);
  friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
  // ---------------- aritmetic operators
  friend Matrix &operator-(Matrix &matrix);
  friend Matrix &operator+(double value, Matrix &matrix);
  friend Matrix &operator+(Matrix &matrix, double value);
  friend Matrix &operator+(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator-(double value, Matrix &matrix);
  friend Matrix &operator-(Matrix &matrix, double value);
  friend Matrix &operator-(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator*(double value, Matrix &matrix);
  friend Matrix &operator*(Matrix &matrix, double value);
  friend Matrix &operator*(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator/(double value, Matrix &matrix);
  friend Matrix &operator/(Matrix &matrix, double value);
  friend Matrix &operator/(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator%(Matrix &matrix, Matrix &other);
  Matrix &T(void);
  // ---------------- boolean operators
  friend Matrix &operator!=(double value, Matrix &matrix);
  friend Matrix &operator!=(Matrix &matrix, double value);
  friend Matrix &operator!=(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator==(double value, Matrix &matrix);
  friend Matrix &operator==(Matrix &matrix, double value);
  friend Matrix &operator==(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator<=(double value, Matrix &matrix);
  friend Matrix &operator<=(Matrix &matrix, double value);
  friend Matrix &operator<=(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator<(double value, Matrix &matrix);
  friend Matrix &operator<(Matrix &matrix, double value);
  friend Matrix &operator<(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator>=(double value, Matrix &matrix);
  friend Matrix &operator>=(Matrix &matrix, double value);
  friend Matrix &operator>=(Matrix &matrix, Matrix &other); //matrix2matrix
  friend Matrix &operator>(double value, Matrix &matrix);
  friend Matrix &operator>(Matrix &matrix, double value);
  friend Matrix &operator>(Matrix &matrix, Matrix &other); //matrix2matrix
  Matrix &to_zeros(void);                                  //returns a matrix of zeros
  void to_csv(string filename);
  void from_csv(string filename);
};

int Matrix::MATRIX_PRINT_PRECISION = 4;
bool Matrix::MATRIX_VERBOSE = false;
bool Matrix::MATRIX_EXIT_IF_ERROR = true;
void Matrix::set_dims(int rows, int cols)
{ //private method
  this->nrows = rows;
  this->ncols = cols;
}
Matrix::Matrix() {}
Matrix::Matrix(int nrows, int ncols)
{
  if (nrows * ncols > 1250000) // 10MB of memory
  {
    char continue_;
    cout << "\033[1;93m:warning - Matrix::Matrix(int,int)" << endl;
    cout << "\033[0;93m:warning - Matrix size is huge! More than 10MB.\033[m" << endl;
    cout << "\033[0;93m:warning - Are you sure about this size? It may cause computer brake or crash.\033[m" << endl;
    cout << "\033[0;93m:warning - Do you want continue? [y/n]\033[m" << endl;
    cin >> continue_;
    if (MATRIX_EXIT_IF_ERROR and ((continue_ != 'y') and (continue_ != 'Y')))
    {
      cout << "\033[0;93m:exiting\033[m" << endl;
      exit(1);
    }
  }

  register int i;
  this->me = (double **)calloc(nrows, sizeof(double *));
  if (this->me == nullptr)
  {
    cout << "\033[1;91m:error - Matrix::Matrix(int,int)" << endl;
    cout << "\033[0;91m:error - Parent pointer could not be allocated!\033[m" << endl;
    throw "Allocation Error ocurred.";
    if (MATRIX_EXIT_IF_ERROR)
    {
      exit(1);
    }
  }
  for (i = 0; i < nrows; i++)
  {
    this->me[i] = (double *)calloc(ncols, sizeof(double));
    if (this->me[i] == nullptr)
    {
      cout << "\033[91m" << i
           << "-th child pointer could not be allocated!\033[m" << endl;
      for (; i == 0; i--)
      {
        free(this->me[i]);
      }
      free(this->me);
      exit(1);
    }
  }

  this->nrows = nrows;
  this->ncols = ncols;
  if (Matrix::MATRIX_VERBOSE)
  {
    cout << "\033[1;95m:ok - Matrix::Matrix(int,int) " << endl;
    cout << "\033[0;95m:ok - Matrix allocation :: size("
         << nrows << ", " << ncols << ")\033[m" << endl;
  }
}

Matrix::~Matrix()
{
  if (Matrix::MATRIX_VERBOSE)
  {
    cout << "\033[1;95m:ok - Matrix::~Matrix()" << endl;
    cout << "\033[0;95m:ok - Matrix destructor called.\033[m" << endl;
  }
  for (register int i = 0; i < this->nrows; i++)
  {
    free(this->me[i]);
  }
  free(this->me);
}

double Matrix::get(int i, int j)
{
  i = validate_index(1, i);
  j = validate_index(2, j);
  return this->me[i][j];
}

void Matrix::set(int i, int j, double value)
{
  i = validate_index(1, i);
  j = validate_index(2, j);
  this->me[i][j] = value;
}

int Matrix::cols() { return this->ncols; }
int Matrix::rows() { return this->nrows; }

int Matrix::validate_index(int ndim, int index)
{
  if (ndim == 1)
  {
    if (index <= -rows() || index >= rows())
    {
      cout << "\033[91m:error - Invalid 'row' index access!" << endl;
      cout << ":error - The valid range is [" << 1 - nrows << ", " << nrows - 1
           << "]. The index accessed is " << index << "." << endl;
      cout << ":exiting\033[m" << endl;
      exit(1);
    }

    return ((index + this->nrows) % this->nrows);
  }
  else
  {
    if (index <= -cols() || index >= cols())
    {
      cout << "\033[91m:error - Invalid 'col' index access!" << endl;
      cout << ":error - The valid range is [" << 1 - this->ncols << ", " << this->ncols - 1
           << "]. The index accessed is " << index << "." << endl;
      cout << ":exiting\033[m" << endl;
      exit(1);
    }
    return ((index + this->ncols) % this->ncols);
  }
}

std::ostream &operator<<(std::ostream &out, Matrix &matrix)
{
  // Since operator<< is a friend of the Matrix class, we can access Matrix's members directly.
  out << "\033[96;1m";
  if (Matrix::MATRIX_VERBOSE)
  {
    out << ":ok - Printing ";
  }
  out << "Matrix matrix(" << matrix.rows() << ", " << matrix.cols() << ")" << endl;
  out << "[" << endl;
  for (register int i = 0; i < matrix.rows(); i++)
  {
    out << "  ";
    for (register int j = 0; j < matrix.cols(); j++)
    {
      out << setw(Matrix::MATRIX_PRINT_PRECISION + 5) << setprecision(Matrix::MATRIX_PRINT_PRECISION) << matrix.get(i, j) << " ";
    }
    out << endl;
  }
  out << "]\033[m" << endl;

  return out; // return std::ostream so we can chain calls to operator<<
}

Matrix &operator+(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) + value);
    }
  }
  return *ans;
}
Matrix &operator+(Matrix &matrix, double value)
{
  return operator+(value, matrix);
}

Matrix &operator-(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, value - matrix.get(i, j));
    }
  }
  return *ans;
}
Matrix &operator-(Matrix &matrix, double value)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) - value);
    }
  }
  return *ans;
}
//////////////////////////////////////////////////////////////////////q

Matrix &operator/(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, value / matrix.get(i, j));
    }
  }
  return *ans;
}
Matrix &operator/(Matrix &matrix, double value)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) / value);
    }
  }
  return *ans;
}

Matrix &operator*(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) * value);
    }
  }
  return *ans;
}
Matrix &operator*(Matrix &matrix, double value)
{
  return operator*(value, matrix);
}
// ----------------- BOOLEAN OPERATORS
Matrix &operator!=(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) != value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator!=(Matrix &matrix, double value)
{
  return operator!=(value, matrix);
}

Matrix &operator==(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) == value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator==(Matrix &matrix, double value)
{
  return operator==(value, matrix);
}

Matrix &operator<=(double value, Matrix &matrix)
{

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (value <= matrix.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator<=(Matrix &matrix, double value)
{

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) <= value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator<(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (value < matrix.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator<(Matrix &matrix, double value)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) < value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator>=(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (value >= matrix.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator>=(Matrix &matrix, double value)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) >= value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator>(double value, Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (value > matrix.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}
Matrix &operator>(Matrix &matrix, double value)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) > value)
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator-(Matrix &matrix)
{
  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      ans->set(i, j, -matrix.get(i, j));
    }
  }
  return *ans;
}

Matrix &operator%(Matrix &matrix, Matrix &other)
{
  if (matrix.cols() != other.rows())
  {
    cout << "\033[91m:error - Matrix multiplication not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", \033[5m"
         << matrix.cols() << "\033[0;91m) x (\033[5m" << other.rows() << "\033[0;91m, " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(matrix.rows(), other.cols());
  register int i, j, k;
  double sum;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < other.cols(); j++)
    {
      sum = 0.0;
      for (k = 0; k < matrix.cols(); k++)
      {
        sum += matrix.get(i, k) * other.get(k, j);
      }
      ans->set(i, j, sum);
    }
  }
  return *ans;
}

Matrix &Matrix::T(void)
{ // function matrix transposition
  register int i, j;
  Matrix *ans = new Matrix(this->cols(), this->rows());
  for (i = 0; i < this->rows(); i++)
  {
    for (j = 0; j < this->cols(); j++)
    {

      ans->set(j, i, this->get(i, j));
    }
  }
  return *ans;
}

Matrix &operator+(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix sum not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(matrix.rows(), other.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < other.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) + other.get(i, j));
    }
  }
  return *ans;
}

Matrix &operator-(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix subtraction not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(matrix.rows(), other.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < other.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) - other.get(i, j));
    }
  }
  return *ans;
}

Matrix &operator*(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix multiplication \"entry-entry\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(matrix.rows(), other.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < other.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) * other.get(i, j));
    }
  }
  return *ans;
}

Matrix &operator/(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix division \"entry-entry\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(matrix.rows(), other.cols());
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < other.cols(); j++)
    {
      ans->set(i, j, matrix.get(i, j) / other.get(i, j));
    }
  }
  return *ans;
}

Matrix &operator!=(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) != other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator==(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) == other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator>=(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) >= other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator<=(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) <= other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator>(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) > other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &operator<(Matrix &matrix, Matrix &other)
{
  if (matrix.rows() != other.rows() || matrix.cols() != other.cols())
  {
    cout << "\033[91m:error - Matrix comparison \"!=\" not allowed!" << endl;
    cout << ":error - Dimensions do not match: (" << matrix.rows() << ", "
         << matrix.cols() << ") x (" << other.rows() << ", " << other.cols()
         << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }

  Matrix *ans = new Matrix(matrix.rows(), matrix.cols());

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      if (matrix.get(i, j) < other.get(i, j))
      {
        ans->set(i, j, 1);
      }
      else
      {
        ans->set(i, j, 0);
      }
    }
  }
  return *ans;
}

Matrix &Matrix::to_zeros()
{
  Matrix *ans = new Matrix(this->rows(), this->cols());
  return *ans;
}

Matrix &eye(int rows, int cols, int offset = 0)
{
  if (rows <= 0 || cols <= 0)
  {
    cout << "\033[91m:error - Matrix dimension not allowed!" << endl;
    cout << ":error - Dimensions must be strictly positive! Got (" << rows << ", "
         << cols << ")." << endl;
    cout << ":exiting\033[m" << endl;
    exit(1);
  }
  Matrix *ans = new Matrix(rows, cols);

  register int i, j;

  for (i = 0; i < rows; i++)
  {
    j = i + offset;
    if (j < 0 || j >= cols)
    {
      continue;
    }
    ans->set(i, j, 1.0);
  }
  return *ans;
}

Matrix &eye(int n)
{
  return eye(n, n);
}

void Matrix::to_csv(string filename)
{

  string extension_csv;
  if (filename.size() < 5)
  {
    extension_csv = ".";
  }
  else
  {
    extension_csv = filename.substr(filename.size() - 4, 4);
  }
  if (extension_csv.compare(".csv") != 0)
  { //filename does NOT have extension .csv
    filename.append(".csv");
    if (MATRIX_VERBOSE)
    {
      cout << "\033[1;93m:warning - Matrix::to_csv(string)" << endl;
      cout << "\033[0;93m:warning - Filename does NOT have extension '.csv'." << endl;
      cout << ":warning - I am appending this extension into the file name.\033[m" << endl;
    }
  }

  ofstream csv_file;
  csv_file.open(filename);
  if (!csv_file.is_open())
  {
    throw "Error: File could not be opened.";
    if (MATRIX_VERBOSE)
    {
      cout << "\033[1;91m:error - Matrix::to_csv(string)" << endl;
      cout << "\033[0;91m:error - File '" << filename << "' could not be opened." << endl;
      if (MATRIX_EXIT_IF_ERROR)
      {
        cout << "\033[0;91m:exiting" << endl;
        exit(1);
      }
    }
  }
  else
  { // writing the csv file
    csv_file << rows() << "," << cols() << " //matrix dimensions" << endl;
    for (register int i = 0; i < rows(); i++)
    {
      for (register int j = 0; j < cols() - 1; j++)
      {
        csv_file << setprecision(15) << this->get(i, j) << ",";
      }
      csv_file << setprecision(15) << this->get(i, cols() - 1) << endl;
    }
    csv_file.close();
  }
}

void Matrix::from_csv(string filename)
{
  string extension_csv;
  if (filename.size() < 5)
  {
    extension_csv = ".";
  }
  else
  {
    extension_csv = filename.substr(filename.size() - 4, 4);
  }
  if (extension_csv.compare(".csv") != 0)
  { //filename does NOT have extension .csv
    filename.append(".csv");
    if (MATRIX_VERBOSE)
    {
      cout << "\033[1;93m:warning - Matrix::to_csv(string)" << endl;
      cout << "\033[0;93m:warning - Filename does NOT have extension '.csv'." << endl;
      cout << ":warning - I am appending this extension into the file name.\033[m" << endl;
    }
  }

  ifstream csv_file;

  csv_file.open(filename);
  if (!csv_file.is_open())
  {

    cout << "\033[1;91m:error - Matrix::to_csv(string)" << endl;
    cout << "\033[0;91m:error - File '" << filename << "' could not be opened." << endl;

    cout << "\033[0;91m:exiting" << endl;
    exit(1);
  }
  else
  {

    string line;
    vector<double> values_vector;

    bool found;
    size_t index = 0;
    double value;
    while (getline(csv_file, line))
    {
      index = line.find("//");
      if (index != string::npos)
      {
        line = line.erase(index, line.size() - 1); //????? -1
      }
      found = true;
      while (found && line.size() > 0)
      {
        index = line.find(",");
        if (index != string::npos)
        {
          found = true;

          try
          {
            value = stod(line.substr(0, index));
          }
          catch (exception &e)
          {
            cout << "\033[91;1m:error - Matrix::from_csv(string)" << endl;
            cout << "\033[0;91m:error - I could not convert some value. Verify the file." << endl;
            cout << ":error - file name: " << filename << endl;
            cout << ":error - " << e.what() << endl;
            cout << ":exiting\033[m" << endl;
          }
          values_vector.push_back(value);
          line.erase(0, index + 1);
        }
        else
        {
          found = false;
        }
      }
      if (line.size() > 0)
      {
        try
        {
          value = stod(line);
          values_vector.push_back(value);
        }
        catch (exception &e)
        {
          cout << "\033[91;1m:error - Matrix::from_csv(string)" << endl;
          cout << "\033[0;91m:error - I could not convert some value. Verify the file." << endl;
          cout << ":error - file name: " << filename << endl;
          cout << ":error - " << e.what() << endl;
          cout << ":exiting\033[m" << endl;
          exit(1);
        }
      }
    }
    int rows_, cols_;
    try
    {
      rows_ = (int)values_vector[0];
      cols_ = (int)values_vector[1];
      values_vector.erase(values_vector.begin());
      values_vector.erase(values_vector.begin());
    }
    catch (exception &e)
    {
      cout << "\033[91;1m:error - Matrix::from_csv(string)" << endl;
      cout << "\033[0;91m:error - I think you passed a blank file. Verify it." << endl;
      cout << ":error - file name: " << filename << endl;
      cout << ":error - " << e.what() << endl;
      cout << ":exiting\033[m" << endl;
      exit(1);
    }

    if (rows_ * cols_ != values_vector.size())
    {
      cout << "\033[91;1m:error - Matrix::from_csv(string)" << endl;
      cout << "\033[0;91m:error - Matrix dimensions and its values do not match." << endl;
      cout << ":error - file name: " << filename << endl;
      cout << ":exiting\033[m" << endl;
      exit(1);
    }

    Matrix *ans = new Matrix(rows_, cols_);

    register int counter = 0;
    for (register int i = 0; i < rows_; i++)
    {
      for (register int j = 0; j < cols_; j++)
      {
        ans->set(i, j, (double)values_vector[counter]);
        counter += 1;
      }
    }
    *this = *ans;
  }
}
