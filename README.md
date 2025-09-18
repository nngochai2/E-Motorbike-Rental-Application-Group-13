# E-Motorbike Rental Application - Group 13

## Course Information
**EEET2482/EEET2653/COSC2082/COSC2721 GROUP PROJECT**  
**Software Engineering Design & Advanced Programming Techniques**  
**Semester 2, 2025**  
**RMIT University**

**Instructors:** Dr Ling Huo Chong, Dr Ushik Shrestha, Dr Tri Huynh

## Group Members
- **s3978281** - Nguyen Ngoc Hai
- **s3978823** - Tran Dinh Dong
- **s3927474** - Hoang Vinh Khue

## Project Overview

This console-based C++ application simulates a peer-to-peer electric motorbike rental system in response to Vietnam's 
shift toward electric mobility. The system allows users to register as members, list their electric motorbike for rent, 
browse and book other members’ motorbikes, top up credit points (CPs), and provide ratings after each completed ride, 
demonstrating object-oriented programming principles and system design.

## Features

### User Roles
- **Guest**: Browse general motorbike listings (limited information)
- **Member**: Full access to rental system, motorbike management, and transactions
- **Admin**: Complete system oversight and management capabilities

### Authentication & Registration
- **Secure Registration**: Strong password enforcement with validation
- **Profile Management**: Update personal information, manage license details
- **Identity Verification**: Advanced verification system with verified status
- **Default Account**: New members receive 20 credit points and 3.0 rating

### Motorbike Management
- **One Motorbike Per Member**: Each member can register one electric motorbike
- **Flexible Listing**: Set availability periods, daily rates, and minimum renter ratings
- **Smart Filtering**: Search by date, city (Hanoi/HCMC), engine size, and requirements
- **Dynamic Unlisting**: Remove listings unless already booked

### Credit Point System
- **Secure Transactions**: Password-authenticated CP top-ups ($1 = 1 CP)
- **Automatic Deductions**: CPs deducted upon rental confirmation
- **Balance Protection**: Prevents negative balances

### Rental Process
- **Smart Eligibility**: Automatic filtering based on rating, CPs, and license requirements
- **License Restrictions**: Members without valid licenses cannot rent >50cc motorbikes
- **Request Management**: Submit, review, and approve rental requests
- **Conflict Resolution**: Automatic rejection of overlapping requests

### Rating System
- **Mutual Ratings**: Both renter and owner rate each other after rental completion
- **Comprehensive Reviews**: 1-5 star ratings with detailed comments
- **Dynamic Updates**: Ratings automatically averaged and updated over time
- **Performance Tracking**: Separate ratings for members and motorbikes

### Activity Dashboard
- **Account Overview**: Current credit points, ratings, and verification status
- **Active Bookings**: Real-time rental status and history
- **Rental Requests**: Track incoming and outgoing requests
- **Statistics**: Comprehensive system and personal analytics

## Technical Implementation

### Architecture
- **Object-Oriented Design**: Full OOP implementation with proper encapsulation
- **Modular Structure**: Logically separated source files with clear responsibilities
- **Manager Pattern**: Dedicated managers for authentication, motorbikes, and rentals
- **Data Persistence**: Individual file-based storage for efficient updates

### Data Management
- **CSV-Based Storage**: Human-readable data format
- **Automatic Initialization**: Pre-populated test data for immediate demonstration
- **Data Consistency**: Validation and cleanup mechanisms
- **Cross-Platform Compatibility**: Works across different build environments

### Key Components
- **AuthManager**: User authentication and account management
- **MotorbikeManager**: Motorbike registration, listing, and search
- **RentalManager**: Rental requests, bookings, and completion
- **DataManager**: File I/O, data persistence, and consistency
- **InputHelper**: User input validation and error handling

## Quick Start

### Prerequisites
- C++ compiler with C++17 support
- CMake 3.10 or higher

### Building the Project
```bash
# Clone the repository
git clone [repository-url]
cd E-Motorbike-Rental-Application-Group-13

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./Group13_Program.exe  # Windows
./Group13_Program      # Linux/macOS
```

### Default Test Accounts

**Admin Account:**
- Username: `admin`
- Password: `12345`

**Sample Member Accounts:**
- **alice01** / `alicepass` - Has motorbike MB001 (Honda CBR500R)
- **bob22** / `bobpass22` - Has motorbike MB002 (Yamaha YZF-R3)
- **eve55** / `evepass55` - Renter without motorbike
- **grace77** / `gracepass77` - Member without valid license

## Project Structure
```
├── src/
│   ├── entities/          # Core business entities
│   │   ├── User.cpp/h
│   │   ├── Member.cpp/h
│   │   ├── Admin.cpp/h
│   │   ├── Guest.cpp/h
│   │   └── Motorbike.cpp/h
│   ├── managers/          # Business logic managers
│   │   ├── AuthManager.cpp/h
│   │   ├── MotorbikeManager.cpp/h
│   │   ├── RentalManager.cpp/h
│   │   └── DataManager.cpp/h
│   ├── utils/            # Utility classes
│   │   ├── InputHelper.cpp/h
│   │   ├── FileHandler.cpp/h
│   │   ├── DateUtil.cpp/h
│   │   └── DataInitializer.cpp/h
│   ├── enums/            # System enumerations
│   │   ├── RentalStatus.h
│   │   ├── RequestStatus.h
│   │   └── RatingType.h
│   ├── core/             # Application core
│   │   └── Application.cpp/h
│   └── main.cpp          # Entry point
├── data/                 # Auto-generated data files
└── docs/                 # Documentation
```

## Key Constraints & Business Rules

1. **Credit Points**: Balances cannot go negative
2. **One Motorbike Rule**: Each member can register only one motorbike
3. **Concurrent Rentals**: Members can rent only one motorbike at a time
4. **License Requirements**: Valid license required for motorbikes >50cc
5. **No Cancellations**: Rental requests cannot be cancelled once accepted
6. **Mutual Ratings**: Both parties must rate each other after rental completion

## Advanced Features

### Identity Verification
- Creative verification mechanism implementation
- Verified status display for trusted members
- Enhanced security for high-value transactions

### Analytics Dashboard
- Personalized member dashboard with real-time data
- System-wide statistics and trends
- Performance metrics and insights

### Date Range Filtering
- Enhanced search with date range specification
- Availability checking for entire rental periods
- Smart conflict detection and resolution

## Data Consistency & Validation

- **Input Validation**: Comprehensive validation for all user inputs
- **Data Integrity**: Automatic validation of relationships between entities
- **Error Handling**: Graceful error handling with informative messages
- **Cleanup Mechanisms**: Orphaned file detection and cleanup

## Testing & Demonstration

The application includes comprehensive test data demonstrating:
- Various member types (with/without motorbikes, different license states)
- Active rentals and historical transactions
- Rating relationships and comment systems
- Complex scenarios for feature demonstration

## Submission Compliance

This project meets all RMIT submission requirements:
- Standalone executable that runs immediately
- Complete source code with proper file organization
- Comprehensive documentation and comments
- Object-oriented design principles
- Data persistence and loading mechanisms
- Professional presentation and user experience

## Academic Integrity

This project represents original work by Group 13 members and complies with RMIT's academic integrity policies. All external resources used are properly acknowledged in the source code comments.

## Future Enhancements

Potential areas for expansion:
- Web-based interface
- Mobile application
- Payment gateway integration
- GPS tracking for motorbikes
- Advanced analytics and reporting
- Multi-language support

## Contact Information

For questions about this project, please contact any group member through RMIT email or during the scheduled Q&A session.

---

**Project Submission Date:** September 18, 2025  
**RMIT University - School of Science, Engineering and Technology (SSET)**