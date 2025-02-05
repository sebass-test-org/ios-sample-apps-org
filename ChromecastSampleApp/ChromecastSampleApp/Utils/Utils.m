//
//  Utils.m
//  ChromecastSampleApp
//
//  Created on 9/18/14.
//  Copyright © 2014 Ooyala, Inc. All rights reserved.
//

#import "Utils.h"
#import <CommonCrypto/CommonHMAC.h>

@implementation Utils

+ (NSData *)getDataFromImageURL:(NSString *)imgURL{
  if (!imgURL) {
    return nil;
  }

  NSFileManager *fileManager = NSFileManager.defaultManager;
  NSString *cacheFileName = [self sha1HashForString:imgURL];
  NSURL *cacheDirectory = [[[fileManager URLsForDirectory:NSCachesDirectory inDomains:NSUserDomainMask] lastObject] URLByAppendingPathComponent:@"thumbnails"];
  NSURL *cacheFileURL = [cacheDirectory URLByAppendingPathComponent:cacheFileName];

  if ([fileManager fileExistsAtPath:cacheFileURL.path]) {
    //Cache hit
    NSLog(@"cache log hit file to URL = %@", cacheFileURL);
    return [[NSData alloc] initWithContentsOfURL:cacheFileURL];
  } else {
    // Retrive the data from the internet
    NSData *imageData = [[NSData alloc] initWithContentsOfURL:[[NSURL alloc] initWithString:imgURL]];
    // Create the cache directorty, if needed
    NSError *error;
    if (![fileManager fileExistsAtPath:cacheDirectory.path]) {
      [fileManager createDirectoryAtURL:cacheDirectory withIntermediateDirectories:YES attributes:nil error:&error];

      if (error) {
        NSLog(@"Received an error while trying to create a directory %@", error.localizedDescription);
      }
    }

    error = nil;
    // Write the image to our cache
    NSLog(@"cache log writing file to URL = %@", cacheFileURL);
    [imageData writeToURL:cacheFileURL options:NSDataWritingAtomic error:&error];
    if (error) {
      NSLog(@"Received an error while trying to save a cached file %@", error.localizedDescription);
    }
    return imageData;
  }
}

+ (NSString *)sha1HashForString:(NSString *)input {
  NSData *data = [input dataUsingEncoding:NSUTF8StringEncoding];
  uint8_t digest[CC_SHA1_DIGEST_LENGTH];
  CC_SHA1(data.bytes, (CC_LONG)data.length, digest);

  NSMutableString *hash = [NSMutableString stringWithCapacity:40];
  for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++) {
    [hash appendFormat:@"%02x", digest[i]];
  }

  return hash;
}

@end
