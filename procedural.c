#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define INIT_SIZE 20
typedef enum { ITEM, DOCUMENT } AssetType;
typedef enum { LAPTOP, ID_CARD } ItemType;
typedef enum { FINANCE, IT, HR } DocumentType;
typedef struct {
  AssetType asset_type;
  union {
    ItemType item_type;
    DocumentType document_type;
  };
  char name[50];
  char asignee[50];
} Asset;
typedef struct {
  Asset* assets;
  size_t size;
  size_t capacity;
} AssetList;

AssetList* new_asset_list() {
  AssetList* list = malloc(sizeof(AssetList));
  if (list == NULL) return NULL;
  list->assets = malloc(INIT_SIZE * sizeof(Asset));
  if (list->assets == NULL) {
    free(list);
    return NULL;
  }
  list->size = 0;
  list->capacity = INIT_SIZE;
  return list;
}

int free_asset_list(AssetList* list) {
  if (list == NULL) return -1;
  free(list->assets);
  free(list);
  return 0;
}

int display_asset_list(const AssetList* list) {
  if (list == NULL || list->assets == NULL) return -1;

  for (size_t i = 0; i < list->size; i++) {
    const Asset* asset = &list->assets[i];
    const char* asset_type_str =
        (asset->asset_type == ITEM) ? "Item" : "Document";
    const char* specific_type_label = "Item";
    const char* specific_type_str;

    if (asset->asset_type == ITEM) {
      specific_type_str = (asset->item_type == LAPTOP) ? "Laptop" : "ID Card";
    } else {
      specific_type_label = "Division";
      switch (asset->document_type) {
        case FINANCE:
          specific_type_str = "Finance";
          break;
        case IT:
          specific_type_str = "IT";
          break;
        case HR:
          specific_type_str = "HR";
          break;
        default:
          specific_type_str = "Unknown";
          break;
      }
    }
    printf("Asset %zu: Type: %s, %s: %s, Name: %s, Assignee: %s\n", i + 1,
           asset_type_str, specific_type_label, specific_type_str, asset->name,
           asset->asignee);
  }
  return 0;
}

int push_asset(AssetList* list, Asset asset) {
  if (list == NULL || list->assets == NULL) return -1;
  if (list->size >= list->capacity) {
    size_t new_capacity = list->capacity * 2;
    Asset* new_assets = realloc(list->assets, new_capacity * sizeof(Asset));
    if (new_assets == NULL) return -1;
    list->assets = new_assets;
    list->capacity = new_capacity;
  }
  list->assets[list->size++] = asset;
  return 0;
}

int remove_asset(AssetList* list, size_t index) {
  if (list == NULL || list->assets == NULL || index >= list->size) return -1;
  size_t elements_to_shift = list->size - index - 1;
  if (elements_to_shift > 0) {
    memmove(&list->assets[index], &list->assets[index + 1],
            elements_to_shift * sizeof(Asset));
  }
  list->size--;
  return 0;
}

int main() {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  /**
   * Start simulation flow
   * 1. Create an asset list.
   * 2. Add 3 assets to the list.
   * 3. Display the asset list.
   * 4. Remove the second asset from the list.
   * 5. Display the asset list again.
   * 6. Free the asset list.
   * 7. Measure the elapsed time for the operations.
   */
  AssetList* assets = new_asset_list();
  if (assets == NULL) {
    fprintf(stderr, "Failed to create asset list.\n");
    return EXIT_FAILURE;
  }
  if (push_asset(assets, (Asset){ITEM, .item_type = LAPTOP, "Dell XPS 13", "Alice"}) != 0 ||
      push_asset(assets, (Asset){ITEM, .item_type = ID_CARD, "ID12345", "Bob"}) != 0 ||
      push_asset(assets, (Asset){DOCUMENT, .document_type = IT, "IT Policy", "Charlie"}) != 0
    ) {
    fprintf(stderr, "Failed to add assets.\n");
    free_asset_list(assets);
    return EXIT_FAILURE;
  }
  if (display_asset_list(assets) != 0) {
    fprintf(stderr, "Failed to display asset list.\n");
    free_asset_list(assets);
    return EXIT_FAILURE;
  }
  if (remove_asset(assets, 1) != 0) {
    fprintf(stderr, "Failed to remove asset.\n");
    free_asset_list(assets);
    return EXIT_FAILURE;
  }
  if (display_asset_list(assets) != 0) {
    fprintf(stderr, "Failed to display asset list after removal.\n");
    free_asset_list(assets);
    return EXIT_FAILURE;
  }
  if (free_asset_list(assets) != 0) {
    fprintf(stderr, "Failed to free asset list.\n");
    return EXIT_FAILURE;
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("Elapsed time: %.0f microseconds\n", (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3);
  return 0;
}