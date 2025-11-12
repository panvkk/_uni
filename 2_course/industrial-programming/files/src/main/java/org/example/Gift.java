package org.example;

public class Gift {
    private String name;
    private double price;

    public Gift(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }

    @Override
    public String toString() {
        return String.format("%s (%.2f руб.)", name, price);
    }
}