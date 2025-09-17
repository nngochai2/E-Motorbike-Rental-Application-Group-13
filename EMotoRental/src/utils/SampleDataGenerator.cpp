//
// Created by Tran Dinh Dong on 9/17/2025.
//
#include "SampleDataGenerator.h"
#include "../managers/AuthManager.h"
#include "../entities/Admin.h"
#include <iostream>

namespace EMotoRental::SampleDataGenerator {
    void addDefaultAdmins(AuthManager& authManager) {
        // Check if at least one of the defaults exists
        if (authManager.findAdminByUsername("admin01") != nullptr) {
            std::cout << "Default admins already exist, skipping seeding." << std::endl;
            return;
        }

        Admin* admin1 = new Admin("admin01", "adminpass01", "Michael Scott");
        Admin* admin2 = new Admin("admin02", "securepass02", "Sarah Parker");
        Admin* admin3 = new Admin("admin03", "masterkey03", "James Taylor");
        Admin* admin4 = new Admin("admin04", "admin1234", "Linda White");
        Admin* admin5 = new Admin("admin05", "rootpass05", "Robert Green");

        authManager.addAdmin(admin1);
        authManager.addAdmin(admin2);
        authManager.addAdmin(admin3);
        authManager.addAdmin(admin4);
        authManager.addAdmin(admin5);

        std::cout << "Default admins created successfully." << std::endl;
    }
}

