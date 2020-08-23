#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>


using namespace std;

class Date {
public:
    Date (int new_year, int new_month, int new_day)//стандарт даты
    {

        if (new_month < 1 || new_month > 12)
        {
            throw out_of_range("Month value is invalid: " + to_string(new_month));
        } else if (new_day < 1 || new_day > 31)
        {
            throw out_of_range("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const{
        return year;
    };
    int GetMonth() const{
        return month;
    };
    int GetDay() const{
        return day;
    };
private:
    int year;
    int month;
    int day;
};

ostream& operator<< (ostream& stream, const Date& date) //Печатаем дату
{
    stream << setfill('0') << setw(4) << to_string(date.GetYear());
    stream << "-";
    stream << setfill('0') << setw(2)<< to_string(date.GetMonth());
    stream << "-";
    stream <<  setfill('0')  << setw(2) << to_string(date.GetDay());
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs){
    if (lhs.GetYear() == rhs.GetYear()){
        if(lhs.GetMonth() == rhs.GetMonth()){
            return lhs.GetDay() < rhs.GetDay();
        }
        else{
            return lhs.GetMonth() < rhs.GetMonth();
        }
    }
    else{
        return lhs.GetYear() < rhs.GetYear();
    }
};

Date CreateDate(const string& date) //преобразуем строку в класс Дата
{
    istringstream date_stream(date);
    bool istrue = true;

    int year;
    istrue = istrue && (date_stream >> year);
    istrue = istrue && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    istrue = istrue && (date_stream >> month);
    istrue = istrue && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    istrue = istrue && (date_stream >> day);
    istrue = istrue && date_stream.eof();

    if (!istrue)
    {
        throw logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}


class Database{
public:
    void AddEvent(const Date& date, const string& event) {
        if (!event.empty()) {
            db[date].insert(event);
        }
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (db.count(date) > 0 && db[date].count(event) > 0) {
            db[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date &date) {
        int s = 0;
        if (db.count(date) > 0) {
            s = db.at(date).size();
            db.erase(date);
        }
        return s;
    }

    set<string> Find(const Date &date) const {
        set<string> deals;
        if (db.count(date) > 0) {
            deals = db.at(date);
        }
        return deals;
    }

    void Print() const {
        for (const auto &x : db) {
            for (auto s : x.second) {
                cout << x.first << ' ' << s << endl;
            }
        }
    }

private:
    map<Date, set<string>> db;
};

int main() {
    Database db;

    try {
        Database db;
        string command;

        while (getline(cin, command)) {
            if (command == "") continue;

            stringstream input(command);

            string operation;
            input >> operation;

            if (operation == "Add") {
                string date_str, deal;
                input >> date_str >> deal;
                const Date date = CreateDate(date_str);
                if (deal != "" || deal != " ") {
                    db.AddEvent(date, deal);
                }
            } else if (operation == "Find") {
                string date_str, deal;
                input >> date_str >> deal;
                const Date date = CreateDate(date_str);
                set<string> deals = db.Find(date);

                for (string e : deals) {
                    cout << e << endl;
                }
            } else if (operation == "Del") {
                string date_str, deal;
                input >> date_str;
                if (!input.eof()) {
                    input >> deal;
                }

                const Date date = CreateDate(date_str);

                if (deal.empty()) {
                    int num_of_events_to_del = db.DeleteDate(date);
                    cout << "Deleted " << num_of_events_to_del << " events" << endl;
                } else {
                    if (db.DeleteEvent(date, deal)) {
                        cout << "Deleted successfully" << endl;
                    } else {
                        cout << "Event not found" << endl;
                    }
                }

            } else if (operation == "Print") {
                db.Print();
            } else {
                cout << "Unknown command: " << operation << endl;
            }
        }
    }
    catch (exception& our_exc)
    {
        cout << our_exc.what() << endl;
        return 0;
    }
    return 0;
}




