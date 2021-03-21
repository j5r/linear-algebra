#pragma once
#include "./matrix.cpp"

Matrix &conjugate_gradient(Matrix &matrix, Matrix &b, double tol = 1e-8)
{
  int max_ = b.rows();

  Matrix *x = new Matrix(max_, 1);
  Matrix *r = new Matrix();
  Matrix *v = new Matrix();
  Matrix *z = new Matrix();
  Matrix *aux = new Matrix();
  Matrix *aux1 = new Matrix();
  Matrix *s = new Matrix();
  Matrix *m = new Matrix();
  r->copy(b);
  v->copy(b);
  *aux = (r->T()) % (*r);

  for (register int i = 0; i < max_; i++)
  {
    *z = matrix % (*v);
    *s = (*aux) / (v->T() % (*z));
    *x = (*x) + (*s) % (*v);
    *r = (*r) - (*s) % (*z);
    *aux1 = (r->T()) % (*r);
    if (aux1->get(0, 0) < tol)
    {
      if (Matrix::MATRIX_VERBOSE)
      {
        cout << "\033[1;92m:ok - la::conjugate_gradient(Matrix,Matrix,double)" << endl;
        cout << "\033[0;92m:ok - Solution reached at " << i + 1 << "/" << max_ << " iteration." << endl;
        cout << "\033[m";
      }
      return *x;
    }
    *m = (*aux1) / (*aux);
    aux->~Matrix();
    *aux = *aux1;
    *v = (*r) + (*m) % (*v);
  }
  return *x;
}