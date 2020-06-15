#include <iostream>

class Create {

private:
    long long value;

public:
    Create(long long alt) {
        value = alt;
    }

    Create(Create const &alt) {
        value = alt.value;
    }

    Create() {
        value = 0;
    }

    long long get_alt() {
        return value;
    }

    void operator=(Create alt1) {
        value = alt1.value;
    }

    Create operator++() {
        return Create(++value);
    }

    Create operator++(int) {
        return Create(value++);
    }

    Create operator--() {
        return Create(--value);
    }

    Create operator--(int) {
        return Create(value--);
    }

    void operator+=(Create alt1) {
        value += alt1.value;
    }

    void operator-=(Create alt1) {
        value -= alt1.value;
    }

    void operator*=(Create alt1) {
        value *= alt1.value;
    }

    void operator/=(Create alt1) {
        value /= alt1.value;
    }

    void operator%=(Create alt1) {
        value %= alt1.value;
    }

    ~Create() = default;
};

Create operator+(Create alt1, Create alt2) {
    Create current = alt1;
    current += alt2;
    return current;
}

Create operator-(Create alt1, Create alt2) {
    Create current = alt1;
    current -= alt2;
    return current;
}

Create operator*(Create alt1, Create alt2) {
    Create current = alt1;
    current *= alt2;
    return current;
}

Create operator/(Create alt1, Create alt2) {
    Create current = alt1;
    current /= alt2;
    return current;
}

Create operator%(Create &alt1, Create &alt2) {
    Create current = alt1;
    current %= alt2;
    return current;
}

bool operator==(Create alt1, Create alt2) {
    return alt1.get_alt() == alt2.get_alt();
}

bool operator<(Create alt1, Create alt2) {
    return alt1.get_alt() < alt2.get_alt();
}

bool operator>(Create alt1, Create alt2) {
    return alt1.get_alt() > alt2.get_alt();
}

bool operator<=(Create alt1, Create alt2) {
    return alt1.get_alt() <= alt2.get_alt();
}

bool operator>=(Create alt1, Create alt2) {
    return alt1.get_alt() >= alt2.get_alt();
}


std::ostream &operator<<(std::ostream &s, Create alt1) {
    s << alt1.get_alt();
    return s;
}

std::istream &operator>>(std::istream &s, Create &alt1) {
    long long num;
    s >> num;
    alt1 = num;
    return s;
}


int main() {
    Create first_alt;
    Create second_alt;
    std::cin >> first_alt;
    std::cin >> second_alt;
    std::cout <<"first_alt + second_alt = " << (first_alt + second_alt);
    return 0;
}
