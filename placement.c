#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

/* Files */
const char *files[] = {
    "6+SERVICE.txt",    /* index 0 -> 6.x */
    "7+STARTUP.txt",    /* index 1 -> 7.x */
    "8+UNICORNS.txt",   /* index 2 -> 8.x */
    "9.0+MAANG.txt"     /* index 3 -> 9.x - 10.x */
};

/* Markers for 6.x subranges */
const char *markers6[] = {
    "--$2-4--",   /* 6.00 - 6.25 */
    "--$3-7--",   /* 6.26 - 6.50 */
    "--$5-9--",   /* 6.51 - 6.75 */
    "--$10+--"    /* 6.76 - 7.00 */
};

/* Niche options and markers for 7.x (11 markers) */
const char *niches7[] = {
    "Fintech & Payments",
    "E-commerce & Marketplaces",
    "Edtech",
    "SaaS & Enterprise",
    "Logistics & Delivery",
    "Gaming & Entertainment",
    "HealthTech & MedTech",
    "Travel & Mobility",
    "Foodtech & Consumer",
    "Manufacturing & B2B",
    "PropTech & Others"
};
const char *niche_markers7[] = {
    "####$# Fintech & Payments ####$#",
    "####$# E-commerce & Marketplaces ####$#",
    "####$# Edtech ####$#",
    "####$# SaaS & Enterprise ####$#",
    "####$# Logistics & Delivery ####$#",
    "####$# Gaming & Entertainment ####$#",
    "####$# HealthTech & MedTech ####$#",
    "####$# Travel & Mobility ####$#",
    "####$# Foodtech & Consumer ####$#",
    "####$# Manufacturing & B2B ####$#",
    "####$# PropTech & Others ####$#"
};

/* Niche options and markers for 8.x (6 markers) */
const char *niches8[] = {
    "Fintech & Payments",
    "Investment and Finance Giants",
    "Cloud, Enterprise and Infrastructure",
    "Developer, Productivity and Platforms",
    "Streaming, Social Media",
    "Marketplaces, Travel, Gaming and Hardware"
};
const char *niche_markers8[] = {
    "####$# Fintech & Payments ####$#",
    "####$# Investment and Finance Giants ####$#",
    "####$# Cloud, Enterprise and Infrastructure ####$#",
    "####$# Developer, Productivity and Platforms ####$#",
    "####$# Streaming, Social Media ####$#",
    "####$# Marketplaces, Travel, Gaming and Hardware ####$#"
};

/* Location options and markers for 9.x - 10.x */
const char *locations[] = {
    "Washington",
    "California",
    "Texas"
};
const char *location_markers[] = {
    "+))washington((",
    "+))california((",
    "+))texas(("
};

/* Utility: read a double safely */
double get_double_in_range(const char *prompt, double min, double max) {
    char buf[128];
    char *end;
    double val;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) return -1.0; /* EOF or error */
        val = strtod(buf, &end);
        if (end == buf || (*end != '\n' && *end != '\0')) {
            printf("Invalid number. Try again.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Out of range. Enter a value between %.2f and %.2f.\n", min, max);
            continue;
        }
        return val;
    }
}

/* Utility: read an integer choice safely */
int get_int_choice(const char *prompt, int min, int max) {
    char buf[64];
    char *end;
    long val;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) return -1;
        val = strtol(buf, &end, 10);
        if (end == buf || (*end != '\n' && *end != '\0')) {
            printf("Invalid input. Enter a number.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Out of range. Enter between %d and %d.\n", min, max);
            continue;
        }
        return (int)val;
    }
}

/* Print the section between two identical markers (first occurrence -> next occurrence) */
void show_section_between_markers(const char *filename, const char *marker) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror(filename);
        return;
    }

    char line[1024];
    bool printing = false;
    bool found_start = false;

    while (fgets(line, sizeof line, fp)) {
        if (strstr(line, marker)) {
            /* Toggle printing on first marker, stop after second marker */
            printing = !printing;
            if (printing) {
                found_start = true;
                continue; /* do not print the marker line itself */
            } else {
                /* we saw the closing marker; stop reading */
                break;
            }
        }
        if (printing) {
            fputs(line, stdout);
        }
    }

    if (!found_start) {
        printf("Marker '%s' not found in %s\n", marker, filename);
    }

    fclose(fp);
}

/* Show whole file (helper) */
void show_whole_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror(filename); return; }
    char line[1024];
    while (fgets(line, sizeof line, fp)) fputs(line, stdout);
    fclose(fp);
}

/* Choose marker for 6.x based on fractional ranges */
const char *choose_marker_for_6(double cgpa) {
    /* assume cgpa in [6.0, 7.0] */
    if (cgpa <= 6.25) return markers6[0];
    if (cgpa <= 6.50) return markers6[1];
    if (cgpa <= 6.75) return markers6[2];
    return markers6[3]; /* up to 7.00 */
}

void placement () {
    printf("CGPA-based viewer (6.0 - 10.0)\n");

    double cgpa = get_double_in_range("Enter CGPA (6.0 - 10.0): ", 6.0, 10.0);
    if (cgpa < 0) { printf("Please Calculate CGPA option 4 \n"); return ; }

    if (cgpa >= 6.0 && cgpa < 7.0) {
        const char *filename = files[0]; /* 6+SERVICE.txt */
        const char *marker = choose_marker_for_6(cgpa);
        printf("\nFile: %s\nMarker: %s\n\n", filename, marker);
        show_section_between_markers(filename, marker);
    }
    else if (cgpa >= 7.0 && cgpa < 8.0) {
        const char *filename = files[1]; /* 7+STARTUP.txt */
        printf("\nFile: %s\nChoose niche:\n", filename);
        int n = (int)(sizeof niches7 / sizeof niches7[0]);
        for (int i = 0; i < n; ++i) printf("  %d) %s\n", i + 1, niches7[i]);
        printf("  0) Show whole file\n");

        int choice = get_int_choice("Select option: ", 0, n);
        if (choice == 0) show_whole_file(filename);
        else show_section_between_markers(filename, niche_markers7[choice - 1]);
    }
    else if (cgpa >= 8.0 && cgpa < 9.0) {
        const char *filename = files[2]; /* 8+UNICORNS.txt */
        printf("\nFile: %s\nChoose niche:\n", filename);
        int n = (int)(sizeof niches8 / sizeof niches8[0]);
        for (int i = 0; i < n; ++i) printf("  %d) %s\n", i + 1, niches8[i]);
        printf("  0) Show whole file\n");

        int choice = get_int_choice("Select option: ", 0, n);
        if (choice == 0) show_whole_file(filename);
        else show_section_between_markers(filename, niche_markers8[choice - 1]);
    }
    else { /* 9.0 - 10.0 */
        const char *filename = files[3]; /* 9.0+MAANG.txt */
        printf("\nFile: %s\nChoose location:\n", filename);
        int n = (int)(sizeof locations / sizeof locations[0]);
        for (int i = 0; i < n; ++i) printf("  %d) %s\n", i + 1, locations[i]);
        printf("  0) Show whole file\n");

        int choice = get_int_choice("Select option: ", 0, n);
        if (choice == 0) show_whole_file(filename);
        else show_section_between_markers(filename, location_markers[choice - 1]);
    }

}
