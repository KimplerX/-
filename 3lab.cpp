#include <iostream>
#include <string>

using namespace std;

enum class PaymentMethod { CARD, CASH };

struct Customer {
    double personalDiscountPercent = 0.0;
};

struct Purchase {
    double basePrice;
    string category;
    int purchaseHour; 
    PaymentMethod paymentMethod;
    const Customer& customer;
};


double applyNightDiscount(double price, const Purchase& p) {
    if (p.purchaseHour >= 23 || p.purchaseHour < 6) {
        cout << "  -> Applied 5% night discount\n";
        return price * 0.95; 
    }
    return price;
}

double applyCategoryDiscount(double price, const Purchase& p) {
    if (p.category == "Electronics") {
        cout << "  -> Applied 15% 'Electronics' category discount\n";
        return price * 0.85; 
    }
    return price;
}

double applyCardDiscount(double price, const Purchase& p) {
    if (p.paymentMethod == PaymentMethod::CARD) {
        cout << "  -> Applied 3% card payment discount\n";
        return price * 0.97; 
    }
    return price;
}

double applyPersonalDiscount(double price, const Purchase& p) {
    double percent = p.customer.personalDiscountPercent;
    if (percent > 0) {
        cout << "  -> Applied " << percent << "% personal discount\n";
        return price * (1.0 - percent / 100.0);
    }
    return price;
}


double calculateFinalPrice(const Purchase& p) {
    double price = p.basePrice;
    cout << "Base Price: " << price << endl;

    price = applyCategoryDiscount(price, p);
    price = applyNightDiscount(price, p);
    price = applyCardDiscount(price, p);
    price = applyPersonalDiscount(price, p);
    
    return price;
}

int main() {
    Customer vipCustomer{10.0};
    Customer regularCustomer{0.0};

    cout << "--- DEMO 1: Laptop, Night, Card, VIP Customer ---\n";
    Purchase p1{10000.0, "Electronics", 3, PaymentMethod::CARD, vipCustomer};
    double finalPrice1 = calculateFinalPrice(p1);
    cout << "*** Final Price: " << finalPrice1 << " ***\n\n";
    
    cout << "--- DEMO 2: Book, Day, Cash, Regular Customer ---\n";
    Purchase p2{800.0, "Books", 14, PaymentMethod::CASH, regularCustomer};
    double finalPrice2 = calculateFinalPrice(p2);
    cout << "*** Final Price: " << finalPrice2 << " ***\n\n";

    cout << "--- DEMO 3: Headphones, Day, Card, Regular Customer ---\n";
    Purchase p3{2000.0, "Electronics", 16, PaymentMethod::CARD, regularCustomer};
    double finalPrice3 = calculateFinalPrice(p3);
    cout << "*** Final Price: " << finalPrice3 << " ***\n";

    return 0;
}