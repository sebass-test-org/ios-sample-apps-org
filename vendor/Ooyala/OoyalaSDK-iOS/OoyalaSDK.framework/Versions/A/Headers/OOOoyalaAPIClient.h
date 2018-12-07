/**
 * @class      OOOoyalaAPIClient OOOoyalaAPIClient.h "OOOoyalaAPIClient.h"
 * @brief      OOOoyalaAPIClient
 * @details    OOOoyalaAPIClient.h in OoyalaSDK
 * @date       1/17/12
 * @copyright Copyright (c) 2015 Ooyala, Inc. All rights reserved.
 */

@import Foundation;
#import "OOSecureURLGenerator.h"
#import "OOCallbacks.h"
#import "OOPaginatedParentItem.h"
#import "OOEmbedTokenGenerator.h"

@class OOPlayerAPIClient;
@class OOOoyalaAPIHelper;
@class OOContentItem;
@class OOOoyalaError;
@class OOPlayerDomain;

/**
 * Ooyala API client implementation.
 * Used internally by the OOOoyalaPlayer to load content information and metadata.
 * Can be used by the customer app to query Backlot APIs
 */
@interface OOOoyalaAPIClient : NSObject

/**
 * Initialize an OOOoyalaAPIClient with pcode and embed domain
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPcode:(NSString *)pcode domain:(OOPlayerDomain *)domain;

/**
 * Initialize an OOOoyalaAPIClient with pcode, embed domain and OOEmbedTokenGenerator.
 *
 * Allows accessing content that is protected by Ooyala Player Tokens.
 *
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @param[in] generator OOEmbedTokenGenerator for creating Ooyala Player Tokens
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPcode:(NSString *)pcode
                       domain:(OOPlayerDomain *)domain
          embedTokenGenerator:(id<OOEmbedTokenGenerator>)generator;

/**
 * Initialize an OOOoyalaAPIClient with pcode, embed domain and OOSecureURLGenerator
 *
 * Allows accessing protected Backlot APIs
 *
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @param[in] secureURLGenerator an initialized instance of OOSecureURLGenerator used for signing Backlot requests
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPcode:(NSString *)pcode
                       domain:(OOPlayerDomain *)domain
           secureUrlGenerator:(id<OOSecureURLGenerator>)secureURLGenerator;


/**
 * Initialize an OOOoyalaAPIClient with pcode, embed domain, API key and secret
 *
 * Allows accessing protected Backlot APIs.
 * Use OOOoyalaAPIClient::initWithPcode:domain:secureUrlGenerator: if you don't want to embed API keys in the app.
 *
 * @param[in] apiKey the API Key to use (from Backlot)
 * @param[in] secret the Secret to use (from Backlot)
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithAPIKey:(NSString *)apiKey
                        secret:(NSString *)secret
                         pcode:(NSString *)pcode
                        domain:(OOPlayerDomain *)domain;

/**
 * Initialize an OOOoyalaAPIClient with pcode, embed domain, API key and secret and OOEmbedTokenGenerator
 *
 * Allows accessing content that is protected by Ooyala Player Tokens.
 * Allows accessing protected Backlot APIs.
 * Use OOOoyalaAPIClient::initWithPcode:domain:embedTokenGenerator:secureUrlGenerator: if you don't want to embed API keys in the app.
 *
 * @param[in] apiKey the API Key to use (from Backlot)
 * @param[in] secret the Secret to use (from Backlot)
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @param[in] generator the initialized OOEmbedTokenGenerator to use
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithAPIKey:(NSString *)apiKey
                        secret:(NSString *)secret
                         pcode:(NSString *)pcode
                        domain:(OOPlayerDomain *)domain
           embedTokenGenerator:(id<OOEmbedTokenGenerator>)generator;

/**
 * Initialize an OOOoyalaAPIClient with pcode, embed domain, OOEmbedTokenGenerator and OOSecureURLGenerator
 *
 * Allows accessing content that is protected by Ooyala Player Tokens.
 * Allows accessing protected Backlot APIs.
 *
 * @param[in] pcode the Partner Code to use (from Backlot)
 * @param[in] domain the embed domain
 * @param[in] generator the initialized OOEmbedTokenGenerator to use
 * @param[in] secureURLGenerator an initialized instance of OOSecureURLGenerator used for signing Backlot requests
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPcode:(NSString *)pcode
                       domain:(OOPlayerDomain *)domain
          embedTokenGenerator:(id<OOEmbedTokenGenerator>)generator
           secureUrlGenerator:(id<OOSecureURLGenerator>)secureURLGenerator;


/** @internal
 * Initialize an OOOoyalaAPIClient, able to sign Backlot requests
 * @param[in] playerAPI the initialized OOPlayerAPIClient to use
 * @param[in] secureURLGenerator an initialized instance of OOSecureURLGenerator used for signing Backlot requests
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPlayerAPIClient:(OOPlayerAPIClient *)playerAPI
                     secureUrlGenerator:(id<OOSecureURLGenerator>)secureURLGenerator;

/** @internal
 * Initialize an OOOoyalaAPIClient, able to sign Backlot requests
 * @param[in] thePlayerAPIClient the initialized OOPlayerAPIClient to use
 * @returns the initialized OOOoyalaAPIClient
 */
- (instancetype)initWithPlayerAPIClient:(OOPlayerAPIClient *)thePlayerAPIClient;

/**
 * Asynchronously fetch the content tree for a set of embed codes
 * @param[in] embedCodes an NSArray containing the embed codes to fetch the content tree for
 * @param[in] callback the OOContentTreeCallback to execute when the asynchronous fetch completes
 */
- (void)contentTree:(NSArray *)embedCodes
           callback:(OOContentTreeCallback)callback;

/**
 * Asynchronously fetch the content tree for a set of embed codes, with the specified ad set
 * @param[in] embedCodes an NSArray containing the embed codes to fetch the content tree for
 * @param[in] adSetCode (possibly nil) an NSString containing the ad set code for the ad set to dynamically assign
 * @param[in] callback the OOContentTreeCallback to execute when the asynchronous fetch completes
 */
- (void)contentTree:(NSArray *)embedCodes
          adSetCode:(NSString *)adSetCode
           callback:(OOContentTreeCallback)callback;

/**
 * Asynchronously fetch the content tree for a set of external ids
 * @note: The external ids will not be in the resulting OOContentItem tree. All ContentItems are keyed based on embed code.
 * @param[in] externalIds an NSArray containing the external ids to fetch the content tree for
 * @param[in] callback the OOContentTreeCallback to execute when the asynchronous fetch completes
 */
- (void)contentTreeByExternalIds:(NSArray *)externalIds
                        callback:(OOContentTreeCallback)callback;

/**
 * Asynchronously fetch next part of the content tree if content tree is too large
 * Use OOPaginatedParentItem::hasMoreChildren to check if this is needed.
 * @param[in,out] parent the OOPaginatedParentItem to fetch more children for
 * @param[in] callback a block called on return
 */
- (void)contentTreeNext:(id<OOPaginatedParentItem>)parent
               callback:(OOContentTreeNextCallback)callback;

/**
 * Asynchronously fetch a raw NSDictionary/NSArray from any backlot API (GET requests only)
 * @param[in] uri the URI to be fetched from backlot *not* including "/v2". For example, to request https://api.ooyala.com/v2/assets, uri should be "/assets"
 * @param[in] params Additional params that the API may require in the form of dictionary.
 * @param[in] callback the OOObjectFromBacklotAPICallback to execute when the asynchronous fetch completes
 */
- (void)objectFromBacklotAPI:(NSString *)uri
                      params:(NSDictionary *)params
                withCallback:(OOObjectFromBacklotAPICallback)callback;

/** @internal
 * Get the provider code that this OOOoyalaAPIClient uses
 * @returns the provider code this OOOoyalaAPIClient uses
 */
- (NSString *)pcode;

/** @internal
 * Get the embed domain that this OOOoyalaAPIClient uses
 * @returns the embed domain this OOOoyalaAPIClient uses
 */
- (OOPlayerDomain *)domain;

+ (NSString *)messageForAuthCode:(int)code;

@end
