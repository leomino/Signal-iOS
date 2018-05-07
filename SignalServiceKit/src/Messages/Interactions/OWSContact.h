//
//  Copyright (c) 2018 Open Whisper Systems. All rights reserved.
//

#import <Mantle/MTLModel.h>

NS_ASSUME_NONNULL_BEGIN

@class CNContact;
@class OWSSignalServiceProtosDataMessage;
@class OWSSignalServiceProtosDataMessage;
@class OWSSignalServiceProtosDataMessageContact;
@class TSAttachment;
@class YapDatabaseReadWriteTransaction;

extern BOOL kIsSendingContactSharesEnabled;

typedef NS_ENUM(NSUInteger, OWSContactPhoneType) {
    OWSContactPhoneType_Home = 1,
    OWSContactPhoneType_Mobile,
    OWSContactPhoneType_Work,
    OWSContactPhoneType_Custom,
};

NSString *NSStringForContactPhoneType(OWSContactPhoneType value);

@protocol OWSContactField <NSObject>

- (BOOL)ows_isValid;

- (NSString *)localizedLabel;

- (NSString *)debugDescription;

@end

#pragma mark -

@interface OWSContactPhoneNumber : MTLModel <OWSContactField>

@property (nonatomic, readonly) OWSContactPhoneType phoneType;
// Applies in the OWSContactPhoneType_Custom case.
@property (nonatomic, readonly, nullable) NSString *label;

@property (nonatomic, readonly) NSString *phoneNumber;

@end

#pragma mark -

typedef NS_ENUM(NSUInteger, OWSContactEmailType) {
    OWSContactEmailType_Home = 1,
    OWSContactEmailType_Mobile,
    OWSContactEmailType_Work,
    OWSContactEmailType_Custom,
};

NSString *NSStringForContactEmailType(OWSContactEmailType value);

@interface OWSContactEmail : MTLModel <OWSContactField>

@property (nonatomic, readonly) OWSContactEmailType emailType;
// Applies in the OWSContactEmailType_Custom case.
@property (nonatomic, readonly, nullable) NSString *label;

@property (nonatomic, readonly) NSString *email;

@end

#pragma mark -

typedef NS_ENUM(NSUInteger, OWSContactAddressType) {
    OWSContactAddressType_Home = 1,
    OWSContactAddressType_Work,
    OWSContactAddressType_Custom,
};

NSString *NSStringForContactAddressType(OWSContactAddressType value);

@interface OWSContactAddress : MTLModel <OWSContactField>

@property (nonatomic, readonly) OWSContactAddressType addressType;
// Applies in the OWSContactAddressType_Custom case.
@property (nonatomic, readonly, nullable) NSString *label;

@property (nonatomic, readonly, nullable) NSString *street;
@property (nonatomic, readonly, nullable) NSString *pobox;
@property (nonatomic, readonly, nullable) NSString *neighborhood;
@property (nonatomic, readonly, nullable) NSString *city;
@property (nonatomic, readonly, nullable) NSString *region;
@property (nonatomic, readonly, nullable) NSString *postcode;
@property (nonatomic, readonly, nullable) NSString *country;

@end

#pragma mark -

@interface OWSContact : MTLModel

@property (nonatomic, readonly, nullable) NSString *givenName;
@property (nonatomic, readonly, nullable) NSString *familyName;
@property (nonatomic, readonly, nullable) NSString *nameSuffix;
@property (nonatomic, readonly, nullable) NSString *namePrefix;
@property (nonatomic, readonly, nullable) NSString *middleName;
@property (nonatomic, readonly, nullable) NSString *organizationName;
@property (nonatomic, readonly) NSString *displayName;

@property (nonatomic, readonly) NSArray<OWSContactPhoneNumber *> *phoneNumbers;
@property (nonatomic, readonly) NSArray<OWSContactEmail *> *emails;
@property (nonatomic, readonly) NSArray<OWSContactAddress *> *addresses;

// TODO: This is provisional.
@property (nonatomic, readonly, nullable) TSAttachment *avatar;
// "Profile" avatars should _not_ be saved to device contacts.
@property (nonatomic, readonly) BOOL isProfileAvatar;

- (instancetype)init NS_UNAVAILABLE;

- (void)normalize;

- (BOOL)ows_isValid;

- (NSString *)debugDescription;

#pragma mark - Creation and Derivation

- (OWSContact *)newContactWithNamePrefix:(nullable NSString *)namePrefix
                               givenName:(nullable NSString *)givenName
                              middleName:(nullable NSString *)middleName
                              familyName:(nullable NSString *)familyName
                              nameSuffix:(nullable NSString *)nameSuffix;

- (OWSContact *)copyContactWithNamePrefix:(nullable NSString *)namePrefix
                                givenName:(nullable NSString *)givenName
                               middleName:(nullable NSString *)middleName
                               familyName:(nullable NSString *)familyName
                               nameSuffix:(nullable NSString *)nameSuffix;

@end

#pragma mark -

@interface OWSContacts : NSObject

#pragma mark - VCard Serialization

+ (nullable CNContact *)systemContactForVCardData:(NSData *)data;
+ (nullable NSData *)vCardDataForSystemContact:(CNContact *)systemContact;

#pragma mark - System Contact Conversion

+ (nullable OWSContact *)contactForSystemContact:(CNContact *)systemContact;
+ (nullable CNContact *)systemContactForContact:(OWSContact *)contact;

#pragma mark -

+ (nullable OWSContact *)contactForVCardData:(NSData *)data;
+ (nullable NSData *)vCardDataContact:(OWSContact *)contact;

#pragma mark - Proto Serialization

+ (nullable OWSSignalServiceProtosDataMessageContact *)protoForContact:(OWSContact *)contact;
+ (OWSContact *_Nullable)contactForDataMessage:(OWSSignalServiceProtosDataMessage *)dataMessage;

@end

NS_ASSUME_NONNULL_END
