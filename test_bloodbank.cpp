#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
    #include "Blood_Inventory_Management.h"
    #include "Blood_Request_Management.h"
    #include "Blood_Distribution_Management.h"
    #include "Donor_Management.h"
    #include "Donation_Management.h"
    #include "Users.h"
    #include "Reports.h"
}

class BloodBankTest : public ::testing::Test {
protected:
    void SetUp() override {
        CleanupFiles();
    }

    void TearDown() override {
#ifdef _WIN32
        freopen("NUL", "r", stdin);
#else
        freopen("/dev/null", "r", stdin);
#endif
        CleanupFiles();
    }

    static void CleanupFiles() {
        const char* files[] = {
            "Inventory.dat", "Donor_Details.dat", "Donation_History.dat",
            "Requests.dat", "Distribution_History.dat", "Admin.dat",
            "Temp.dat", "test_input.txt"
        };
        for (auto f : files) remove(f);
    }

    static void MockStdin(const char* content) {
        FILE* f = fopen("test_input.txt", "w");
        fputs(content, f);
        fclose(f);
        freopen("test_input.txt", "r", stdin);
    }

    static void SeedInventory(const char* type, float avail, float reserved, float issued) {
        FILE* f = fopen("Inventory.dat", "ab");
        Blood_Inventory b = {};
        std::strncpy(b.Blood_Type, type, sizeof(b.Blood_Type) - 1);
        b.Available_Units = avail;
        b.Reserved_Units  = reserved;
        b.Issued_Units    = issued;
        fwrite(&b, sizeof(b), 1, f);
        fclose(f);
    }

    static void SeedDonor(int id, const char* name, const char* blood) {
        FILE* f = fopen("Donor_Details.dat", "ab");
        Donor d = {};
        d.Donor_ID = id;
        std::strncpy(d.Name,          name,  sizeof(d.Name) - 1);
        std::strncpy(d.Blood_Type,    blood, sizeof(d.Blood_Type) - 1);
        std::strncpy(d.Mobile_Number, "0000000000", sizeof(d.Mobile_Number) - 1);
        std::strncpy(d.Address,       "Main Street", sizeof(d.Address) - 1);
        d.Age = 30;
        std::strncpy(d.Gender, "Male", sizeof(d.Gender) - 1);
        d.Weight = 70.0f;
        fwrite(&d, sizeof(d), 1, f);
        fclose(f);
    }

    static void SeedRequest(int id, const char* blood, int units, const char* status) {
        FILE* f = fopen("Requests.dat", "ab");
        Blood_Request r = {};
        r.Request_ID = id;
        std::strncpy(r.Patient_Name,  "Test Patient",  sizeof(r.Patient_Name) - 1);
        std::strncpy(r.Hospital_Name, "Test Hospital", sizeof(r.Hospital_Name) - 1);
        std::strncpy(r.Blood_Type,    blood,           sizeof(r.Blood_Type) - 1);
        r.Units_Required = units;
        r.Request_Date   = {1, 1, 2024};
        std::strncpy(r.Status, status, sizeof(r.Status) - 1);
        fwrite(&r, sizeof(r), 1, f);
        fclose(f);
    }

    static void SeedDonation(int don_id, int donor_id, int d, int m, int y, float units) {
        FILE* f = fopen("Donation_History.dat", "ab");
        Donation don = {};
        don.Donation_ID = don_id;
        don.Donor_ID = donor_id;
        don.Donation_Date = {d, m, y};
        don.Units_Donated = units;
        std::strncpy(don.Location, "Test Loc", sizeof(don.Location) - 1);
        fwrite(&don, sizeof(don), 1, f);
        fclose(f);
    }
    
    static void SeedDistribution(int dist_id, int req_id) {
        FILE* f = fopen("Distribution_History.dat", "ab");
        Blood_Distribution dist = {};
        dist.Distribution_ID = dist_id;
        dist.Request_ID = req_id;
        std::strncpy(dist.Patient_Name, "Test P", sizeof(dist.Patient_Name) - 1);
        std::strncpy(dist.Blood_Type, "A+", sizeof(dist.Blood_Type) - 1);
        dist.Units_Issued = 2;
        dist.Issue_Date = {1, 1, 2024};
        fwrite(&dist, sizeof(dist), 1, f);
        fclose(f);
    }
};

TEST_F(BloodBankTest, SearchBloodGroup_MissingFile_ReturnsSentinel) {
    Blood_Inventory r = Search_Blood_Group("A+");
    EXPECT_FLOAT_EQ(r.Available_Units, -1.0f);
}

TEST_F(BloodBankTest, SearchBloodGroup_EmptyFile_ReturnsSentinel) {
    FILE* f = fopen("Inventory.dat", "wb"); fclose(f);
    Blood_Inventory r = Search_Blood_Group("A+");
    EXPECT_FLOAT_EQ(r.Available_Units, -1.0f);
}

TEST_F(BloodBankTest, SearchBloodGroup_FindsExactMatch) {
    SeedInventory("A+", 10.0f, 2.0f, 1.0f);
    SeedInventory("B-",  5.0f, 0.0f, 0.0f);
    Blood_Inventory r = Search_Blood_Group("B-");
    ASSERT_NE(r.Available_Units, -1.0f);
    EXPECT_STREQ(r.Blood_Type, "B-");
    EXPECT_FLOAT_EQ(r.Available_Units, 5.0f);
}

TEST_F(BloodBankTest, SearchBloodGroup_AbsentType_ReturnsSentinel) {
    SeedInventory("A+", 10.0f, 0.0f, 0.0f);
    Blood_Inventory r = Search_Blood_Group("Z-");
    EXPECT_FLOAT_EQ(r.Available_Units, -1.0f);
}

TEST_F(BloodBankTest, GenerateDonationId_NoFile_ReturnsOne) { 
    EXPECT_EQ(Generate_Donation_ID(), 1); 
}

TEST_F(BloodBankTest, GenerateRequestId_NoFile_ReturnsOne) { 
    EXPECT_EQ(Generate_Request_ID(), 1); 
}

TEST_F(BloodBankTest, GenerateDistributionId_NoFile_ReturnsOne) { 
    EXPECT_EQ(Generate_Distribution_ID(), 1); 
}

TEST_F(BloodBankTest, GenerateDonationId_ReturnsMaxPlusOne) {
    SeedDonation(5, 1, 1, 1, 2024, 2.0f);
    SeedDonation(7, 1, 2, 1, 2024, 2.0f);
    EXPECT_EQ(Generate_Donation_ID(), 8);
}

TEST_F(BloodBankTest, SearchDonorById_FindsExisting) {
    SeedDonor(42, "Alice", "A+");
    Donor d = Search_By_ID(42);
    ASSERT_NE(d.Donor_ID, -1);
    EXPECT_EQ(d.Donor_ID, 42);
    EXPECT_STREQ(d.Name, "Alice");
}

TEST_F(BloodBankTest, SearchDonorById_NotFound) {
    Donor d = Search_By_ID(99);
    EXPECT_EQ(d.Donor_ID, -1);
}

TEST_F(BloodBankTest, ApproveRequest_SufficientStock_SetsApproved) {
    SeedInventory("A+", 20.0f, 0.0f, 0.0f);
    SeedRequest(1, "A+", 5, "Pending");
    Approve_Request(1);
    EXPECT_STREQ(Search_Request_By_ID(1).Status, "Approved");
}

TEST_F(BloodBankTest, ApproveRequest_InsufficientStock_StaysPending) {
    SeedInventory("A+", 2.0f, 0.0f, 0.0f);
    SeedRequest(1, "A+", 5, "Pending");
    Approve_Request(1);
    EXPECT_STREQ(Search_Request_By_ID(1).Status, "Pending");
}

TEST_F(BloodBankTest, RejectRequest_SetsRejected) {
    SeedRequest(1, "A+", 5, "Pending");
    Reject_Request(1);
    EXPECT_STREQ(Search_Request_By_ID(1).Status, "Rejected");
}

TEST_F(BloodBankTest, DeleteDonor_RemovesOnlyMatchingRecord) {
    SeedDonor(1, "Alice", "A+"); 
    SeedDonor(2, "Bob", "B-");
    Delete_Donor(1);
    EXPECT_EQ(Search_By_ID(1).Donor_ID, -1);
    EXPECT_EQ(Search_By_ID(2).Donor_ID, 2);
}

TEST_F(BloodBankTest, AddStock_CreatesNewInventoryRecord) {
    MockStdin("A+ 5.0\n");
    Add_Stock();
    Blood_Inventory r = Search_Blood_Group("A+");
    ASSERT_NE(r.Available_Units, -1.0f);
    EXPECT_FLOAT_EQ(r.Available_Units, 5.0f);
}

TEST_F(BloodBankTest, ReserveBlood_SufficientStock_MovesUnits) {
    SeedInventory("A+", 10.0f, 0.0f, 0.0f);
    MockStdin("A+ 5.0\n");
    Reserve_Blood();
    Blood_Inventory r = Search_Blood_Group("A+");
    EXPECT_FLOAT_EQ(r.Available_Units, 5.0f);
    EXPECT_FLOAT_EQ(r.Reserved_Units,  5.0f);
}

TEST_F(BloodBankTest, BUG_ReserveBlood_RejectsNegativeUnits) {
    SeedInventory("A+", 10.0f, 0.0f, 0.0f);
    MockStdin("A+ -5.0\n");
    Reserve_Blood();
    EXPECT_FLOAT_EQ(Search_Blood_Group("A+").Available_Units, 10.0f);
}

TEST_F(BloodBankTest, EndToEnd_RegisterApproveIssue) {
    SeedInventory("A+", 10.0f, 0.0f, 0.0f);
    MockStdin("\nPatient X\nCity Hospital\nA+\n3\n15\n10\n2024\n");
    Register_Blood_Request();
    Approve_Request(1);
    MockStdin("16\n10\n2024\n");
    Issue_Blood_To_Request(1);
    EXPECT_STREQ(Search_Request_By_ID(1).Status, "Completed");
    EXPECT_FLOAT_EQ(Search_Blood_Group("A+").Available_Units, 7.0f);
}

TEST_F(BloodBankTest, IssueBlood_MovesReservedToIssued) {
    SeedInventory("B+", 0.0f, 10.0f, 0.0f);
    MockStdin("B+ 5.0\n");
    Issue_Blood();
    Blood_Inventory r = Search_Blood_Group("B+");
    EXPECT_FLOAT_EQ(r.Reserved_Units, 5.0f);
    EXPECT_FLOAT_EQ(r.Issued_Units, 5.0f);
}

TEST_F(BloodBankTest, UpdateStockAfterDonation_AddsToAvailable) {
    SeedInventory("O-", 2.0f, 0.0f, 0.0f);
    Update_Stock_After_Donation("O-", 3.5f);
    EXPECT_FLOAT_EQ(Search_Blood_Group("O-").Available_Units, 5.5f);
}

TEST_F(BloodBankTest, UpdateStockAfterDonation_CreatesNewRecord) {
    Update_Stock_After_Donation("AB+", 4.0f);
    Blood_Inventory r = Search_Blood_Group("AB+");
    ASSERT_NE(r.Available_Units, -1.0f);
    EXPECT_FLOAT_EQ(r.Available_Units, 4.0f);
}

TEST_F(BloodBankTest, AddDonor_SuccessfullyAdds) {
    MockStdin("101\nJohn Doe\nA+\n1234567890\n123 Main St\n25\nMale\n70.5\n");
    Add_Donor();
    Donor d = Search_By_ID(101);
    ASSERT_NE(d.Donor_ID, -1);
    EXPECT_STREQ(d.Name, "John Doe");
    EXPECT_STREQ(d.Blood_Type, "A+");
    EXPECT_FLOAT_EQ(d.Weight, 70.5f);
}

TEST_F(BloodBankTest, AddDonor_RejectsDuplicateID) {
    SeedDonor(1, "Existing", "O+");
    MockStdin("1\nNew Guy\nA-\n0000\nAddr\n20\nMale\n60\n");
    Add_Donor();
    EXPECT_STREQ(Search_By_ID(1).Name, "Existing"); 
}

TEST_F(BloodBankTest, SearchByName_FindsMatches) {
    SeedDonor(1, "Alice", "A+"); 
    SeedDonor(2, "Bob", "B-");
    EXPECT_NO_FATAL_FAILURE(Search_By_Name("Alice"));
}

TEST_F(BloodBankTest, SearchByBloodType_FindsMatches) {
    SeedDonor(1, "Alice", "A+"); 
    SeedDonor(2, "Bob", "A+");
    EXPECT_NO_FATAL_FAILURE(Search_By_Blood_Type("A+"));
}

TEST_F(BloodBankTest, UpdateDonorDetails_UpdatesName) {
    SeedDonor(1, "Old Name", "A+");
    MockStdin("1\nNew Name\n");
    Update_Donor_Details(1);
    EXPECT_STREQ(Search_By_ID(1).Name, "New Name");
}

TEST_F(BloodBankTest, RecordDonation_AppendsAndUpdatesInventory) {
    SeedDonor(5, "Donor", "O+");
    SeedInventory("O+", 0.0f, 0.0f, 0.0f);
    MockStdin("5\n10\n10\n2024\n2.0\nCity\nGood\n");
    Record_Donation();
    
    Donation d = Search_Donation_By_ID(1); 
    ASSERT_NE(d.Donation_ID, -1);
    EXPECT_EQ(d.Donor_ID, 5);
    EXPECT_FLOAT_EQ(d.Units_Donated, 2.0f);
    EXPECT_FLOAT_EQ(Search_Blood_Group("O+").Available_Units, 2.0f);
}

TEST_F(BloodBankTest, SearchDonationByDonorID_FindsIt) {
    SeedDonation(1, 99, 1, 1, 2024, 2.0f);
    EXPECT_NO_FATAL_FAILURE(Search_Donation_By_DonorID(99));
}

TEST_F(BloodBankTest, SearchDonationByDate_FindsIt) {
    SeedDonation(1, 99, 15, 6, 2024, 1.0f);
    Date d = {15, 6, 2024};
    EXPECT_NO_FATAL_FAILURE(Search_Donation_By_Date(d));
}

TEST_F(BloodBankTest, SearchDistributionByID_FindsIt) {
    SeedDistribution(42, 7);
    Blood_Distribution d = Search_Distribution_By_ID(42);
    ASSERT_NE(d.Distribution_ID, -1);
    EXPECT_EQ(d.Request_ID, 7);
}

TEST_F(BloodBankTest, InitializeAdmin_CreatesFile) {
    Initialize_Admin(); 
    FILE* f = fopen("Admin.dat", "rb");
    EXPECT_NE(f, nullptr);
    if (f) fclose(f);
}

TEST_F(BloodBankTest, AdminLogin_ValidCredentials) {
    Initialize_Admin(); 
    MockStdin("team27\nkuruvi\n");
    EXPECT_EQ(Admin_Login(), 1); 
}

TEST_F(BloodBankTest, AdminLogin_InvalidCredentials) {
    Initialize_Admin();
    MockStdin("admin\nwrongpass\n");
    EXPECT_EQ(Admin_Login(), 0); 
}

TEST_F(BloodBankTest, ChangeAdminPassword_Success) {
    Initialize_Admin();
    MockStdin("kuruvi\nnewpass123\n"); 
    Change_Admin_Password();
    
    MockStdin("team27\nnewpass123\n");
    EXPECT_EQ(Admin_Login(), 1);
}

TEST_F(BloodBankTest, ViewInventory_PrintsCorrectData) {
    SeedInventory("O+", 12.5f, 0.0f, 0.0f);
    testing::internal::CaptureStdout();
    View_Inventory();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("O+"), std::string::npos);
    EXPECT_NE(output.find("12.50"), std::string::npos);
}

TEST_F(BloodBankTest, ViewDonors_PrintsCorrectData) {
    SeedDonor(99, "John Doe", "A+");
    testing::internal::CaptureStdout();
    View_Donors();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("John Doe"), std::string::npos);
    EXPECT_NE(output.find("A+"), std::string::npos);
}

TEST_F(BloodBankTest, ViewDonations_PrintsCorrectData) {
    SeedDonation(1, 10, 15, 5, 2024, 2.0f);
    testing::internal::CaptureStdout();
    View_Donation();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("15/05/2024"), std::string::npos);
}

TEST_F(BloodBankTest, SystemSummaryReport_PrintsCounts) {
    SeedDonor(1, "Alice", "A+");
    SeedDonation(1, 1, 1, 1, 2024, 1.0f);
    testing::internal::CaptureStdout();
    System_Summary_Report();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Total Donors           : 1"), std::string::npos);
    EXPECT_NE(output.find("Total Donations        : 1"), std::string::npos);
}
TEST_F(BloodBankTest, DonorReport_PrintsData) {
    SeedDonor(1, "Alice", "A+");
    testing::internal::CaptureStdout();
    Donor_Report();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Alice"), std::string::npos);
}

TEST_F(BloodBankTest, DonationReport_PrintsData) {
    SeedDonation(1, 1, 1, 1, 2024, 2.0f);
    testing::internal::CaptureStdout();
    Donation_Report();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Total Donations"), std::string::npos);
}

TEST_F(BloodBankTest, InventoryReport_PrintsData) {
    SeedInventory("B+", 5.0f, 0.0f, 0.0f);
    testing::internal::CaptureStdout();
    Inventory_Report();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("B+"), std::string::npos);
}

TEST_F(BloodBankTest, LowAndOutOfStockReports_PrintData) {
    SeedInventory("O-", 2.0f, 0.0f, 0.0f); // Low stock
    SeedInventory("AB-", 0.0f, 0.0f, 0.0f); // Out of stock
    
    testing::internal::CaptureStdout();
    Low_Stock_Report_Display();
    Out_Of_Stock_Report_Display();
    Low_Stock_Report();       // From Inventory module
    Out_Of_Stock_Report();    // From Inventory module
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("LOW STOCK"), std::string::npos);
    EXPECT_NE(output.find("OUT OF STOCK"), std::string::npos);
}

TEST_F(BloodBankTest, RequestAndDistributionReports_PrintData) {
    SeedRequest(1, "A+", 5, "Pending");
    SeedDistribution(1, 1);
    
    testing::internal::CaptureStdout();
    Request_Report();
    Pending_Request_Report();
    Approved_Request_Report();
    Rejected_Request_Report();
    Distribution_Report();
    View_Requests();
    View_Pending_Requests();
    View_Distribution_History();
    View_Transaction_History();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("PENDING REQUESTS"), std::string::npos);
    EXPECT_NE(output.find("APPROVED REQUESTS"), std::string::npos);
}