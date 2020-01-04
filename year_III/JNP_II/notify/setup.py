from setuptools import setup, find_packages

setup(
    name='notify',
    packages=['notify'],
    include_package_data=True,
    test_suite="tests",
    setup_requires=[
        'pytest-runner',
    ],
    tests_require=[
        'pytest',
    ],
)
