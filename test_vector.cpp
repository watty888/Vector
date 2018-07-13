#include<iostream>
#include<unordered_map>
#include<stdexcept>
#include<algorithm>
#include<numeric>
#include<iterator>
#include"Vector.h"
using namespace std;

class Person {
  string vorname;
  string zuname;
  int alter;
public:
  Person() {} //notwendig um leere Arrayeintraege zu initialisieren
  Person(string vorname, string zuname, int alter) : vorname{vorname}, zuname{zuname}, alter{alter} {}
  virtual ~Person() {}
  string getName() const {return zuname+" "+vorname;}
  int getAlter() const {return alter;}
  void setAlter(int alter) {
    if (alter<0) throw runtime_error("alter muss >= 0 sein!");
    this->alter = alter;
  }
  virtual ostream& print(ostream& o) const {
    return o<<'['<<zuname<<' '<<vorname<<' '<<alter<<']';
  }
};

class Angestellte: public Person {
  int personal_nummer;
public:
  Angestellte() : Person() {} //notwendig, um leere Arrayeintraege zu initialisieren
  Angestellte(string vorname, string zuname, int alter, int personal_nummer) : Person{vorname, zuname, alter}, personal_nummer{personal_nummer} {}
  ostream& print(ostream& o) const override {
    o<<'*'<<personal_nummer<<'*';
    return Person::print(o);
  }
};

ostream& operator<<(ostream&o, const Person& p) {
  return p.print(o);
}

int main() {
  Vector<int> vi {1,2,3,4,5};
  cout << accumulate(vi.begin(), vi.end(), 0, [] (int sum, int elem) { return sum + elem; }) << '\n';

  Vector<string> vs {"a","bb","ccc"};
  vs.erase(find(vs.begin(), vs.end(), "bb"));
  for (auto& s : vs) s+="z";
  for (const auto& s : vs) cout<<s<<' ';
  cout<<'\n';

  Vector<Angestellte> va {Angestellte{"Otto","Mueller",39,5}, Angestellte{"Hans","Bauer",36,1}, Angestellte{"Lisa","Mueller",25,2}, Angestellte{"Maria", "Mayer",40,3}};
  Vector<Person> vp;
  copy(va.begin(), va.end(), std::back_inserter(vp));
  for (const auto& p : va) cout << p << " ";
  cout << '\n';
  for (const auto& p : vp) cout << p << " ";

  cout << "\n\n";
  auto pos = vs.begin();
  transform(va.begin(), va.end(), std::inserter(vs,++pos), [] (const Angestellte& a) {return a.getName();});
  for_each(vs.begin(), vs.end(), [] (const string& s) { cout << s << '\n'; });
  cout<<'\n';

  // Was fehlt in der Klasse Person, damit das folgende Statement kompiliert?
  // Warum fehlt das nicht auch in der Klasse Angestellte?
  //cout << *max_element(va.begin(), va.end());

  // Geben Sie nun analog die juengste Angestellte aus (ohne weitere Aenderungen in Person bzw. Angestellte)

  Vector<string> namenVA;

  return 0;
}

/* Erwartete Ausgabe:
15
az cccz
*5*[Mueller Otto 39] *1*[Bauer Hans 36] *2*[Mueller Lisa 25] *3*[Mayer Maria 40]
[Mueller Otto 39] [Bauer Hans 36] [Mueller Lisa 25] [Mayer Maria 40]

az
Mueller Otto
Bauer Hans
Mueller Lisa
Mayer Maria
cccz

*/
