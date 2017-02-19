#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

class Student
{
  string name;
  string surname;
  bool approved = 0;

  vector<string> subjects;
  vector<double> rating;
  
  public:

  Student(string NAME, string SURNAME) {
    name = NAME;
    surname = SURNAME;
  }

  void
  setName(string val)
  {
    name = val;
  }

  string
  getName()
  {
    return name;
  }

  void
  setSurname(string val)
  {
    surname = val;
  }

  string
  getSurname()
  {
    return surname;
  }

  double
  getRating(size_t subject_id)
  {
    assert(rating.size() > subject_id);
    return rating[subject_id];
  }

  double
  getRating(string subject_title)
  {
    for (size_t i = 0; i < subjects.size(); i++)
      if (subject_title == subjects[i])
        return rating[i];
  }

  void
  raiseRating(size_t subject_id, double value)
  {
    assert(rating.size() > subject_id);
    rating[subject_id] = value;
  }

  void
  raiseRating(string subject_title, double value)
  {
    for (size_t i = 0; i < subjects.size(); i++)
      if (subject_title == subjects[i])
        rating[i] = value;
  }

  void
  printRating()
  {
    for (size_t i = 0; i < subjects.size(); i++)
      cout << subjects[i] << ": " << rating[i] << endl;
  }

  void
  addSubject(string title, double value = 0)
  {
    subjects.push_back(title);
    rating.push_back(value);
  }

  bool
  isApproved(double minimum)
  {
    for(auto& e : rating) {
      if (e < minimum)
        return 0;
    }
    return 1;
  }

};

int
main()
{
  vector<string> subjects { 
    "Math",
    "Programming",
    "English"
  };

  Student alpha ("Ernest", "Shefer");

  for (size_t i = 0; i < subjects.size(); i++)
  {
    alpha.addSubject(subjects[i]);
  }

  alpha.raiseRating("Math", 4);
  alpha.raiseRating(2, 5);

  alpha.printRating();
  
	return 0;
}