#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

class Matrix
{
private:
  /* data */
  int ncols = 0, nrows = 0;
  double **me = nullptr;
  int validate_index(int index, int i);

public:
  static bool MATRIX_VERBOSE;
  static int MATRIX_PRINT_PRECISION;
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
};

int Matrix::MATRIX_PRINT_PRECISION = 4;
bool Matrix::MATRIX_VERBOSE = false;

Matrix::Matrix() {}
Matrix::Matrix(int nrows, int ncols)
{

  register int i;
  this->me = (double **)calloc(nrows, sizeof(double *));
  if (this->me == nullptr)
  {
    cout << "\033[91mParent pointer could not be allocated!\033[m" << endl;
    exit(1);
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
    cout << "\033[95m:ok - Matrix allocation :: size("
         << nrows << ", " << ncols << ")\033[m" << endl;
  }
}

Matrix::~Matrix()
{
  if (Matrix::MATRIX_VERBOSE)
  {
    cout << "\033[95m:ok - Matrix destructor called.\033[m" << endl;
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
    if (index <= -nrows || index >= nrows)
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
    if (index <= -this->ncols || index >= this->ncols)
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
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, matrix.get(i, j) + value);
    }
  }
  return matrix;
}
Matrix &operator+(Matrix &matrix, double value)
{
  return operator+(value, matrix);
}

Matrix &operator-(double value, Matrix &matrix)
{
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, value - matrix.get(i, j));
    }
  }
  return matrix;
}
Matrix &operator-(Matrix &matrix, double value)
{
  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, matrix.get(i, j) - value);
    }
  }
  return matrix;
}

Matrix &operator/(double value, Matrix &matrix)
{

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, value / matrix.get(i, j));
    }
  }
  return matrix;
}
Matrix &operator/(Matrix &matrix, double value)
{

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, matrix.get(i, j) / value);
    }
  }
  return matrix;
}

Matrix &operator*(double value, Matrix &matrix)
{

  register int i, j;
  for (i = 0; i < matrix.rows(); i++)
  {
    for (j = 0; j < matrix.cols(); j++)
    {
      matrix.set(i, j, matrix.get(i, j) * value);
    }
  }
  return matrix;
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
{
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
