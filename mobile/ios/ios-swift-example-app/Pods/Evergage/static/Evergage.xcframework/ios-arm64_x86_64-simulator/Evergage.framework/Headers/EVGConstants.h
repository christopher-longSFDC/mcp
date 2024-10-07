//
//  EVGConstants.h
//  Evergage
//
//  Copyright © 2015-2021 Salesforce.com, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 Indicates the threshold for logging in `<[Evergage logLevel]>`.
 Log messages at the threshold level and lower will be logged.
 For instance, if the threshold is `EVGLogLevelWarn`, messages at level `EVGLogLevelWarn` or `EVGLogLevelError` will be logged.
 @see [Evergage logLevel]
 */
typedef NS_ENUM(NSInteger, EVGLogLevel) {

    /**
     * Indicates that an operation failed.
     */
    EVGLogLevelError = 1000,

    /**
     * Indicates that something abnormal happened but the overall operation did not necessarily fail.
     */
    EVGLogLevelWarn  = 2000,

    /**
     * Provides detail about most operations.
     */
    EVGLogLevelInfo  = 3000,

    /**
     * Provides a high level of detail primarily intended for Evergage developers.
     */
    EVGLogLevelDebug = 4000,

    /**
     * Disables all logging. This is the default for release builds.
     */
    EVGLogLevelOff   = 0,

    /**
     * Enables all logging.
     */
    EVGLogLevelAll   = NSIntegerMax
};

FOUNDATION_EXPORT NSString *const EVGErrorDomain;
