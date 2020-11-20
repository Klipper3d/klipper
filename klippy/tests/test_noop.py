from unittest import TestCase, skip

class TestNoop(TestCase):
    """Placeholder test

    stestr fails with 0 discovered tests.  This provides one to find.
    It can be deleted when other tests are added, but is harmless.
    """

    @skip("No-op test - remove when you've started adding tests")
    def test_noop(self):
        pass
