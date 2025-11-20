plugins {
    id("java")
    id("application")
    id("org.openjfx.javafxplugin") version "0.1.0"
}

group = "org.example"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

java {
    sourceCompatibility = JavaVersion.VERSION_17
    targetCompatibility = JavaVersion.VERSION_17
}

javafx {
    version = "17.0.10"
    modules = listOf("javafx.controls")
}

application {
    mainClass.set("org.example.SortingApplication")
}

dependencies {
    implementation("com.google.code.gson:gson:2.10.1")
}