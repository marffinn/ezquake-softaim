// Download tracking and statistics for ezQuake SoftAim
(function () {
    'use strict';

    const REPO_OWNER = 'marffinn';
    const REPO_NAME = 'ezquake-softaim';
    const API_BASE = 'https://api.github.com/repos';

    // Track download clicks
    function trackDownload(filename) {
        // You can add analytics tracking here if needed
        console.log(`Download tracked: ${filename}`);

        // Optional: Send to analytics service
        if (typeof gtag !== 'undefined') {
            gtag('event', 'download', {
                'event_category': 'engagement',
                'event_label': filename
            });
        }
    }

    // Fetch and display download statistics
    async function updateDownloadStats() {
        try {
            const response = await fetch(`${API_BASE}/${REPO_OWNER}/${REPO_NAME}/releases`);
            const releases = await response.json();

            if (releases && releases.length > 0) {
                let totalDownloads = 0;
                let latestReleaseDownloads = 0;

                // Calculate total downloads across all releases
                releases.forEach((release, index) => {
                    release.assets.forEach(asset => {
                        totalDownloads += asset.download_count;
                        if (index === 0) { // Latest release
                            latestReleaseDownloads += asset.download_count;
                        }
                    });
                });

                // Update DOM elements if they exist
                const totalElement = document.getElementById('total-downloads');
                const latestElement = document.getElementById('latest-downloads');

                if (totalElement) {
                    totalElement.textContent = totalDownloads.toLocaleString();
                }

                if (latestElement) {
                    latestElement.textContent = latestReleaseDownloads.toLocaleString();
                }

                // Update page title with download count
                if (totalDownloads > 0) {
                    document.title = `ezQuake SoftAim (${totalDownloads.toLocaleString()} downloads)`;
                }
            }
        } catch (error) {
            console.error('Failed to fetch download statistics:', error);
        }
    }

    // Add click tracking to download links
    function addDownloadTracking() {
        const downloadLinks = document.querySelectorAll('a[href*="releases"]');
        downloadLinks.forEach(link => {
            link.addEventListener('click', function () {
                const filename = this.href.split('/').pop();
                trackDownload(filename);
            });
        });
    }

    // Initialize when DOM is ready
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', function () {
            updateDownloadStats();
            addDownloadTracking();
        });
    } else {
        updateDownloadStats();
        addDownloadTracking();
    }

    // Update stats every 5 minutes
    setInterval(updateDownloadStats, 5 * 60 * 1000);

})();