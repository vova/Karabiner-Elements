#import "FnFunctionKeysTableViewDelegate.h"
#import "FnFunctionKeysTableViewController.h"
#import "KarabinerKit/KarabinerKit.h"
#import "SimpleModificationsMenuManager.h"
#import "SimpleModificationsTableCellView.h"
#import "SimpleModificationsTableViewController.h"

@interface FnFunctionKeysTableViewDelegate ()

@property(weak) IBOutlet SimpleModificationsMenuManager* simpleModificationsMenuManager;
@property(weak) IBOutlet FnFunctionKeysTableViewController* fnFunctionKeysTableViewController;

@end

@implementation FnFunctionKeysTableViewDelegate

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row {
  if ([tableColumn.identifier isEqualToString:@"FnFunctionKeysFromColumn"]) {
    NSTableCellView* result = [tableView makeViewWithIdentifier:@"FnFunctionKeysFromCellView" owner:self];

    KarabinerKitCoreConfigurationModel* coreConfigurationModel = [KarabinerKitConfigurationManager sharedManager].coreConfigurationModel;
    result.textField.stringValue = [coreConfigurationModel selectedProfileFnFunctionKeyFirstAtIndex:row];

    return result;
  }

  if ([tableColumn.identifier isEqualToString:@"FnFunctionKeysToColumn"]) {
    SimpleModificationsTableCellView* result = [tableView makeViewWithIdentifier:@"FnFunctionKeysToCellView" owner:self];

    result.popUpButton.action = @selector(valueChanged:);
    result.popUpButton.target = self.fnFunctionKeysTableViewController;
    result.popUpButton.menu = [self.simpleModificationsMenuManager.toMenu copy];

    KarabinerKitCoreConfigurationModel* coreConfigurationModel = [KarabinerKitConfigurationManager sharedManager].coreConfigurationModel;
    NSString* toValue = [coreConfigurationModel selectedProfileFnFunctionKeySecondAtIndex:row];
    [SimpleModificationsTableViewController selectPopUpButtonMenu:result.popUpButton representedObject:toValue];

    return result;
  }

  return nil;
}

@end
