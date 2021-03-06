#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

double error = 0.001; //ERROR PARCIAL 2

//struct to store the complex number in case of
struct complex_number {
  double r;
  double i;
};

void division(vector<double> &pol, vector<double> &res, vector<double> &aux, double r, double s) { // Check results on res
  int deg = pol.size() - 1;
  int i, j;
  for(i = deg, j = 0; i >= 0; i--, j++){
    if(i == deg){
      res.at(j) = pol.at(j);
    }
    else if(i == deg - 1){
      res[j] = pol[j] + res[j - 1] * r;
    }
    else{
      res[j] = pol[j] + res[j - 1] * r + res[j - 2] * s;
    }
  }

  for(i = deg, j = 0; i >= 0; i--, j++){
    if(i == deg){
      aux[j] = res[j];
    }
    else if(i == deg - 1){
      aux[j] = res[j] + aux[j - 1] * r;
    }
    else{
      aux[j] = res[j] + aux[j - 1] * r + aux[j - 2] * s;
    }
  }
}

void bairstow(vector<double> &pol, vector<complex_number*> &roots, double pi, double qi, double err) {
  while (pol.size() > 3) {
    vector<double> res(pol.size(), 0.0);
    vector<double> aux(pol.size(), 0.0);

    double p = pi;
    double q = qi;
    double dp = 0.0;
    double dq = 0.0;
    double error_p = 1.0;
    double error_q = 1.0;

    do {
      //Initialize auxiliary arrays as zero.
      for (int i = 0; i < res.size(); i++) {
        res[i] = 0;
        aux[i] = 0;
      }

      division(pol, res, aux, p, q);

      cout << "Result (B) array: {";
      for (int i = 0; i < res.size(); i++) {
        cout << res[i];
        if(i < res.size()-1){
          cout << ", ";
        }
      }
      cout << "}" << endl;

      int deg = pol.size() - 1;
      double d = aux[deg - 2] * aux[deg - 2] - aux[deg - 1] * aux[deg - 3];
      dp = (-res[deg - 1] * aux[deg - 2] + res[deg] * aux[deg - 3]) / d;
      dq = (-aux[deg - 2] * res[deg] + aux[deg - 1] * res[deg - 1]) / d;

      error_p = dp / p; //ERROR PARCIAL 2
      error_q = dq / q; //ERROR PARCIAL 2
      p += dp;
      q += dq;
    } while (abs(error_p) > err && abs(error_q) > err); //ERROR PARCIAL 2

    cout << endl;
    // 2 Roots
    double det = pow(p, 2) + 4 * q;

    complex_number *r1 = new complex_number;
    complex_number *r2 = new complex_number;
    if (det < 0) {
      r1->r = p / 2;
      r1->i = sqrt(-det) / 2;
      r2->r = p / 2;
      r2->i = -sqrt(-det) / 2;
    }
    else {
      r1->r = (p + sqrt(det)) / 2;
      r1->i = 0;
      r2->r = (p - sqrt(det)) / 2;
      r2->i = 0;
    }
    roots.push_back(r1);
    roots.push_back(r2);

    res = vector<double>(pol.size(), 0.0);
    aux = vector<double>(pol.size(), 0.0);

    division(pol, res, aux, p, q);

    // Adjust polynomial
    for (int i = 0; i < pol.size() - 2; i++) {
      pol[i] = res[i];
    }
    pol.pop_back();
    pol.pop_back();


    cout << "Polynomial Update" << endl << "------------------------------" << endl;
    cout << "{";
    for (int i = 0; i < pol.size(); i++) {
      cout << pol[i];
      if(i < pol.size()-1){
        cout << ", ";
      }
    }
    cout << "}" << endl;
  }

  if (pol.size() == 3) {
	double a = pol[0];
    double b = pol[1];
    double c = pol[2];
    double det = pow(b, 2) - (4 * a * c);
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "c = " << c << endl;

    complex_number *r1 = new complex_number;
    complex_number *r2 = new complex_number;
    if (det < 0) {
      r1->r = -b / 2 / a;
      r1->i = sqrt(-det) / 2 / a;
      r2->r = -b / 2 / a;
      r2->i = -sqrt(-det) / 2 / a;
    }
    else {
      r1->r = (-b + sqrt(det)) / (2*a);
      r1->i = 0;
      r2->r = (- b - sqrt(det)) / (2*a);
      r2->i = 0;
    }
    roots.push_back(r1);
    roots.push_back(r2);
  }
  else if (pol.size() == 2) {
    complex_number *r = new complex_number;
    r->r = -pol[1] / pol[0];
    r->i = 0;
    roots.push_back(r);
  }
}

//Array must be in inverse order, where poly_array will be ordered as x^max_degree, ... , x^0.
//g(x) = 0.7 x^3 - 4x^2 + 6.2x - 2
int main(){
  //double poly_array[] = {0.7,-4,6.2,-2};
  double poly_array[] = {4,0,4,0,-1};
  //double poly_array[] = {0.041667, -0.375, 0.78333};
  vector<double> poly (poly_array, poly_array + sizeof(poly_array) / sizeof(double));
  vector<complex_number*> roots;
  double r = -0.4;
  double s = 0.4;
  cout << "Original Polynomial" << endl << "------------------------------" << endl;
  cout << "{";
  for(int i = 0; i < poly.size(); i++) {
    cout << poly[i];
    if(i < poly.size()-1){
      cout << ", ";
    }
  }
  cout << "}" << endl;

  cout << endl << "Bairstow" << endl << "------------------------------" << endl;
  bairstow(poly, roots, r, s, error);

  cout << endl << endl << "Roots" << endl << "------------------------------" << endl;
  for (int i = 0; i < roots.size(); i++) {
    if(roots[i]->i != 0){
      cout << roots[i]->r << " + " << roots[i]->i << "i" << endl;
    }
    else{
        cout << roots[i]->r << endl;
    }
  }
}
