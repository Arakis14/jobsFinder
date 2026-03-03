import os
import time
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from webdriver_manager.chrome import ChromeDriverManager

START_URL = "https://nofluffjobs.com/pl/praca-zdalna/C%2B%2B?criteria=city%3Dhybrid%20%20seniority%3Dmid,senior"

def setup_driver():
    options = webdriver.ChromeOptions()
    options.add_argument("--start-maximized")
    # options.add_argument("--headless")  # uncomment to run without opening browser
    return webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)

def get_job_links(driver, limit=10):
    driver.get(START_URL)

    wait = WebDriverWait(driver, 15)

    # Wait until job links are visible
    wait.until(EC.presence_of_element_located((By.TAG_NAME, "a")))

    time.sleep(5)  # give JS extra time (safer for beginners)

    links = []

    # Find all links on page
    elements = driver.find_elements(By.TAG_NAME, "a")

    for elem in elements:
        href = elem.get_attribute("href")
        if href and "/job/" in href:
            if href not in links:
                links.append(href)
            if len(links) >= limit:
                break

    return links

def save_page(driver, url, filename):
    driver.get(url)
    time.sleep(3)

    html = driver.page_source

    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)

def main():
    driver = setup_driver()

    if not os.path.exists("jobs"):
        os.makedirs("jobs")

    print("Collecting job links...")
    job_links = get_job_links(driver, limit=10)

    print(f"Found {len(job_links)} jobs")

    for i, link in enumerate(job_links, start=1):
        print(f"Saving {link}")
        filename = f"jobs/job_{i}.html"
        save_page(driver, link, filename)

    driver.quit()
    print("Done!")

if __name__ == "__main__":
    main()