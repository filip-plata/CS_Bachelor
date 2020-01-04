from setuptools import setup

setup(
    name='thresholdalert',
    packages=['thresholdalert'],
    include_package_data=True,
    test_suite="tests",
    install_requires=[
        'flask',
    ],
    setup_requires=[
        'pytest-runner',
    ],
    tests_require=[
        'pytest',
    ],
)
