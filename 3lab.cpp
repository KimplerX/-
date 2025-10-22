#include <iostream>
#include <string>
#include <memory> 
#include <utility> 

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

class IPriceCalculator {
public:
    virtual ~IPriceCalculator() = default;
    virtual double calculatePrice() const = 0;
};

class BasePrice : public IPriceCalculator {
private:
    const Purchase& purchase;

public:
    explicit BasePrice(const Purchase& p) : purchase(p) {
         cout << "Base Price: " << purchase.basePrice << endl;
    }

    double calculatePrice() const override {
        return purchase.basePrice;
    }
};

class Discount : public IPriceCalculator {
protected:
    std::unique_ptr<IPriceCalculator> wrappedCalculator;
    const Purchase& purchase;

public:
    Discount(std::unique_ptr<IPriceCalculator> calculator, const Purchase& p)
        : wrappedCalculator(std::move(calculator)), purchase(p) {}

    double calculatePrice() const override {
        return wrappedCalculator->calculatePrice();
    }
};

class CategoryDiscount : public Discount {
public:
    CategoryDiscount(std::unique_ptr<IPriceCalculator> calculator, const Purchase& p)
        : Discount(std::move(calculator), p) {}

    double calculatePrice() const override {
        double price = wrappedCalculator->calculatePrice();
        
        if (purchase.category == "Electronics") {
            cout << "  -> Applied 15% 'Electronics' category discount\n";
            return price * 0.85;
        }
        return price;
    }
};

class NightDiscount : public Discount {
public:
    NightDiscount(std::unique_ptr<IPriceCalculator> calculator, const Purchase& p)
        : Discount(std::move(calculator), p) {}

    double calculatePrice() const override {
        double price = wrappedCalculator->calculatePrice();
        if (purchase.purchaseHour >= 23 || purchase.purchaseHour < 6) {
            cout << "  -> Applied 5% night discount\n";
            return price * 0.95;
        }
        return price;
    }
};

class CardDiscount : public Discount {
public:
    CardDiscount(std::unique_ptr<IPriceCalculator> calculator, const Purchase& p)
        : Discount(std::move(calculator), p) {}

    double calculatePrice() const override {
        double price = wrappedCalculator->calculatePrice();
        if (purchase.paymentMethod == PaymentMethod::CARD) {
            cout << "  -> Applied 3% card payment discount\n";
            return price * 0.97;
        }
        return price;
    }
};

class PersonalDiscount : public Discount {
public:
    PersonalDiscount(std::unique_ptr<IPriceCalculator> calculator, const Purchase& p)
        : Discount(std::move(calculator), p) {}

    double calculatePrice() const override {
        double price = wrappedCalculator->calculatePrice();
        double percent = purchase.customer.personalDiscountPercent;
        if (percent > 0) {
            cout << "  -> Applied " << percent << "% personal discount\n";
            return price * (1.0 - percent / 100.0);
        }
        return price;
    }
};

int main() {
    Customer vipCustomer{10.0};
    Customer regularCustomer{0.0};

    cout << "--- DEMO 1: Laptop, Night, Card, VIP Customer ---\n";
    Purchase p1{10000.0, "Electronics", 3, PaymentMethod::CARD, vipCustomer};
    
    std::unique_ptr<IPriceCalculator> calculator1 = std::make_unique<BasePrice>(p1);
    calculator1 = std::make_unique<CategoryDiscount>(std::move(calculator1), p1);
    calculator1 = std::make_unique<NightDiscount>(std::move(calculator1), p1);
    calculator1 = std::make_unique<CardDiscount>(std::move(calculator1), p1);
    calculator1 = std::make_unique<PersonalDiscount>(std::move(calculator1), p1);

    double finalPrice1 = calculator1->calculatePrice();
    cout << "*** Final Price: " << finalPrice1 << " ***\n\n";

    
    cout << "--- DEMO 2: Book, Day, Cash, Regular Customer ---\n";
    Purchase p2{800.0, "Books", 14, PaymentMethod::CASH, regularCustomer};
    
    std::unique_ptr<IPriceCalculator> calculator2 = std::make_unique<BasePrice>(p2);
    calculator2 = std::make_unique<CategoryDiscount>(std::move(calculator2), p2);
    calculator2 = std::make_unique<NightDiscount>(std::move(calculator2), p2);
    calculator2 = std::make_unique<CardDiscount>(std::move(calculator2), p2);
    calculator2 = std::make_unique<PersonalDiscount>(std::move(calculator2), p2);

    double finalPrice2 = calculator2->calculatePrice();
    cout << "*** Final Price: " << finalPrice2 << " ***\n\n";


    cout << "--- DEMO 3: Headphones, Day, Card, Regular Customer ---\n";
    Purchase p3{2000.0, "Electronics", 16, PaymentMethod::CARD, regularCustomer};

    std::unique_ptr<IPriceCalculator> calculator3 = std::make_unique<BasePrice>(p3);
    calculator3 = std::make_unique<CategoryDiscount>(std::move(calculator3), p3);
    calculator3 = std::make_unique<NightDiscount>(std::move(calculator3), p3);
    calculator3 = std::make_unique<CardDiscount>(std::move(calculator3), p3);
    calculator3 = std::make_unique<PersonalDiscount>(std::move(calculator3), p3);

    double finalPrice3 = calculator3->calculatePrice();
    cout << "*** Final Price: " << finalPrice3 << " ***\n";

    return 0;
}